#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include "position.h"
#include "move_parser.h"

using namespace athena;

namespace
{

struct Layout
{
    SDL_FRect board;
    SDL_FRect panel;
    SDL_FRect footer;
    float cell;
    bool panel_right;
};

struct RenderMetrics
{
    int window_w;
    int window_h;
    int render_w;
    int render_h;
    float scale_x;
    float scale_y;
};

struct UndoState
{
    Position pos;
    std::array<bool, COLOR_NB> eliminated;
};

constexpr int DEFAULT_W = 1280;
constexpr int DEFAULT_H = 860;
constexpr int MIN_W = 820;
constexpr int MIN_H = 620;

void set_color(SDL_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void fill_rect(SDL_Renderer* renderer, const SDL_FRect& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
{
    set_color(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_rect(SDL_Renderer* renderer, const SDL_FRect& rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
{
    set_color(renderer, r, g, b, a);
    SDL_RenderRect(renderer, &rect);
}

const char* turn_name(Color turn)
{
    switch (static_cast<uint8_t>(turn))
    {
        case Color::Red: return "Red";
        case Color::Blue: return "Blue";
        case Color::Yellow: return "Yellow";
        case Color::Green: return "Green";
        default: return "None";
    }
}

bool is_promotion_rank(Color color, Square sq)
{
    switch (static_cast<uint8_t>(color))
    {
        case Color::Red: return sq.rank() == PROMOTION_RANK;
        case Color::Blue: return sq.file() == PROMOTION_RANK;
        case Color::Yellow: return sq.rank() == (RANK_NB - PROMOTION_RANK - 1);
        case Color::Green: return sq.file() == (FILE_NB - PROMOTION_RANK - 1);
        default: return false;
    }
}

const char* piece_label(Piece piece)
{
    switch (static_cast<uint8_t>(piece))
    {
        case Piece::Pawn: return "P";
        case Piece::Knight: return "N";
        case Piece::Bishop: return "B";
        case Piece::Rook: return "R";
        case Piece::Queen: return "Q";
        case Piece::King: return "K";
        default: return "";
    }
}

void set_piece_color(SDL_Renderer* renderer, Color color)
{
    switch (static_cast<uint8_t>(color))
    {
        case Color::Red: set_color(renderer, 227, 83, 83); break;
        case Color::Blue: set_color(renderer, 98, 143, 248); break;
        case Color::Yellow: set_color(renderer, 231, 197, 90); break;
        case Color::Green: set_color(renderer, 92, 191, 109); break;
        default: set_color(renderer, 210, 210, 210); break;
    }
}

SDL_FRect reset_button_rect(const Layout& l)
{
    constexpr float button_w = 92.0f;
    constexpr float button_h = 28.0f;
    return SDL_FRect{
        l.footer.x + l.footer.w - button_w - 12.0f,
        l.footer.y + 10.0f,
        button_w,
        button_h
    };
}

bool point_in_rect(float x, float y, const SDL_FRect& r)
{
    return x >= r.x && x <= (r.x + r.w) && y >= r.y && y <= (r.y + r.h);
}

Layout compute_layout(int width, int height)
{
    const float margin = 24.0f;
    const float gap = 18.0f;
    const float top_h = 34.0f;
    const float bottom_h = 90.0f;

    const bool wide = width >= static_cast<int>(height * 1.15f);
    const float panel_w = wide ? std::clamp(width * 0.27f, 250.0f, 380.0f) : (width - margin * 2.0f);
    const float panel_h = wide ? (height - margin * 2.0f - bottom_h) : std::clamp(height * 0.22f, 130.0f, 200.0f);

    float board_max_w;
    float board_max_h;
    SDL_FRect panel;

    if (wide)
    {
        panel = SDL_FRect{
            width - margin - panel_w,
            margin,
            panel_w,
            panel_h
        };

        board_max_w = width - margin * 2.0f - panel_w - gap;
        board_max_h = height - margin * 2.0f - top_h - bottom_h;
    }
    else
    {
        panel = SDL_FRect{
            margin,
            height - margin - bottom_h - panel_h,
            panel_w,
            panel_h
        };

        board_max_w = width - margin * 2.0f;
        board_max_h = height - margin * 2.0f - top_h - bottom_h - panel_h - gap;
    }

    const float board_size = std::max(280.0f, std::floor(std::min(board_max_w, board_max_h)));
    const float cell = board_size / 14.0f;

    SDL_FRect board;
    if (wide)
    {
        board = SDL_FRect{ margin, margin + top_h, board_size, board_size };
    }
    else
    {
        board = SDL_FRect{ (width - board_size) * 0.5f, margin + top_h, board_size, board_size };
    }

    SDL_FRect footer{
        margin,
        height - margin - bottom_h,
        width - margin * 2.0f,
        bottom_h
    };

    return Layout{board, panel, footer, cell, wide};
}

RenderMetrics query_render_metrics(SDL_Window* window, SDL_Renderer* renderer)
{
    int ww = 0;
    int wh = 0;
    SDL_GetWindowSize(window, &ww, &wh);

    int rw = ww;
    int rh = wh;
    if (!SDL_GetRenderOutputSize(renderer, &rw, &rh))
    {
        rw = ww;
        rh = wh;
    }

    const float sx = (ww > 0) ? static_cast<float>(rw) / static_cast<float>(ww) : 1.0f;
    const float sy = (wh > 0) ? static_cast<float>(rh) / static_cast<float>(wh) : 1.0f;

    return RenderMetrics{ww, wh, rw, rh, sx, sy};
}

bool square_from_mouse(const Layout& l, float x, float y, Square& out)
{
    const float lx = x - l.board.x;
    const float ly = y - l.board.y;

    if (lx < 0.0f || ly < 0.0f || lx >= l.board.w || ly >= l.board.h) return false;

    const int col = static_cast<int>(lx / l.cell);
    const int row = static_cast<int>(ly / l.cell);
    if (col < 0 || col >= 14 || row < 0 || row >= 14) return false;

    const int file = col + 1;
    const int rank = 14 - row;
    Square sq(rank, file);
    if (sq.stone()) return false;

    out = sq;
    return true;
}

std::string build_uci(Square source, Square target, const Position& pos)
{
    std::string uci = source.uci();
    uci += target.uci();

    const PieceClass pc = pos.board(source);
    if (pc.piece() == Piece::Pawn && is_promotion_rank(pc.color(), target))
    {
        uci += 'q';
    }

    return uci;
}

void draw_text(SDL_Renderer* renderer, float x, float y, const std::string& text)
{
    SDL_RenderDebugText(renderer, x, y, text.c_str());
}

bool has_legal_move_for_color(const Position& pos, Color color)
{
    Position probe = pos;
    probe.set_turn(color);

    for (int sr = 1; sr <= 14; ++sr)
    {
        for (int sf = 1; sf <= 14; ++sf)
        {
            const Square source(sr, sf);
            if (source.stone()) continue;

            const PieceClass attacker = probe.board(source);
            if (attacker == PieceClass::Empty() || attacker == PieceClass::Stone() || attacker.color() != color) continue;

            for (int tr = 1; tr <= 14; ++tr)
            {
                for (int tf = 1; tf <= 14; ++tf)
                {
                    const Square target(tr, tf);
                    if (target.stone()) continue;

                    std::string uci = source.uci();
                    uci += target.uci();

                    Move move;
                    std::string error;
                    if (parse_move_uci(probe, uci, move, &error))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool is_checkmated(const Position& pos, Color color)
{
    return pos.inCheck(color) && !has_legal_move_for_color(pos, color);
}

std::string apply_checkmates_and_skip(Position& pos, std::array<bool, COLOR_NB>& eliminated)
{
    std::string status_suffix;

    for (int i = 0; i < COLOR_NB; ++i)
    {
        const Color turn = pos.turn();
        const uint8_t idx = static_cast<uint8_t>(turn);

        if (eliminated[idx])
        {
            pos.set_turn(turn.next());
            continue;
        }

        if (is_checkmated(pos, turn))
        {
            eliminated[idx] = true;
            if (!status_suffix.empty()) status_suffix += " ";
            status_suffix += std::string(turn_name(turn)) + " is checkmated.";
            pos.set_turn(turn.next());
            continue;
        }

        break;
    }

    return status_suffix;
}

void render_scene(
    SDL_Renderer* renderer,
    const Position& pos,
    const Layout& l,
    Square selected,
    Square last_from,
    Square last_to,
    const std::string& status,
    const std::vector<std::string>& history,
    const std::array<bool, COLOR_NB>& eliminated)
{
    fill_rect(renderer, SDL_FRect{0, 0, 5000, 5000}, 16, 18, 25);

    fill_rect(renderer, SDL_FRect{l.board.x - 10.0f, l.board.y - 10.0f, l.board.w + 20.0f, l.board.h + 20.0f}, 29, 33, 46);

    for (int rank = 14; rank >= 1; --rank)
    {
        for (int file = 1; file <= 14; ++file)
        {
            const int row = 14 - rank;
            const int col = file - 1;
            const Square sq(rank, file);

            SDL_FRect cell{
                l.board.x + l.cell * col,
                l.board.y + l.cell * row,
                l.cell,
                l.cell
            };

            if (sq.stone())
            {
                fill_rect(renderer, cell, 43, 47, 59);
            }
            else
            {
                const bool light = ((rank + file) & 1) == 0;
                if (sq == selected) fill_rect(renderer, cell, 102, 150, 82);
                else if (sq == last_from || sq == last_to) fill_rect(renderer, cell, 86, 105, 148);
                else if (light) fill_rect(renderer, cell, 228, 211, 184);
                else fill_rect(renderer, cell, 153, 116, 84);
            }

            const PieceClass pc = pos.board(sq);
            if (pc == PieceClass::Empty() || pc == PieceClass::Stone()) continue;

            SDL_FRect piece_rect{
                cell.x + l.cell * 0.17f,
                cell.y + l.cell * 0.17f,
                l.cell * 0.66f,
                l.cell * 0.66f
            };

            const bool greyed = eliminated[static_cast<uint8_t>(pc.color())];
            if (greyed) set_color(renderer, 140, 140, 146);
            else set_piece_color(renderer, pc.color());
            SDL_RenderFillRect(renderer, &piece_rect);
            draw_rect(renderer, piece_rect, 20, 22, 28);

            set_color(renderer, 16, 16, 16);
            draw_text(renderer, piece_rect.x + 5.0f, piece_rect.y + 7.0f, piece_label(pc.piece()));
        }
    }

    draw_rect(renderer, l.board, 235, 236, 240);

    fill_rect(renderer, l.panel, 28, 32, 44);
    draw_rect(renderer, l.panel, 64, 71, 92);

    float tx = l.panel.x + 12.0f;
    float ty = l.panel.y + 10.0f;

    draw_text(renderer, tx, ty, std::string("Turn: ") + turn_name(pos.turn()));
    ty += 16.0f;
    draw_text(renderer, tx, ty, "Moves:");
    ty += 14.0f;

    std::vector<std::string> wrapped_history;
    const int max_chars_per_row = std::max(4, static_cast<int>((l.panel.w - 24.0f) / 8.0f));
    std::string line;
    for (int i = 0; i < static_cast<int>(history.size()); ++i)
    {
        const std::string token = (i == 0) ? history[i] : (", " + history[i]);
        if (line.empty() || (static_cast<int>(line.size() + token.size()) <= max_chars_per_row))
        {
            line += token;
        }
        else
        {
            wrapped_history.push_back(line);
            line = token;
        }
    }
    if (!line.empty())
    {
        wrapped_history.push_back(line);
    }

    const int max_rows = std::max(1, static_cast<int>((l.panel.h - 58.0f) / 14.0f));
    const int start_idx = std::max(0, static_cast<int>(wrapped_history.size()) - max_rows);
    for (int i = start_idx; i < static_cast<int>(wrapped_history.size()); ++i)
    {
        draw_text(renderer, tx, ty, wrapped_history[i]);
        ty += 14.0f;
    }

    fill_rect(renderer, l.footer, 23, 27, 37);
    draw_rect(renderer, l.footer, 58, 66, 85);

    draw_text(renderer, l.footer.x + 12.0f, l.footer.y + 10.0f, "Status:");
    draw_text(renderer, l.footer.x + 12.0f, l.footer.y + 26.0f, status);

    const SDL_FRect reset_btn = reset_button_rect(l);
    fill_rect(renderer, reset_btn, 49, 58, 74);
    draw_rect(renderer, reset_btn, 95, 108, 132);
    draw_text(renderer, reset_btn.x + 24.0f, reset_btn.y + 9.0f, "Reset");

    SDL_RenderPresent(renderer);
}

} // namespace

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    int win_w = DEFAULT_W;
    int win_h = DEFAULT_H;
    const SDL_DisplayID display = SDL_GetPrimaryDisplay();
    SDL_Rect usable;
    if (display != 0 && SDL_GetDisplayUsableBounds(display, &usable))
    {
        win_w = std::clamp(static_cast<int>(usable.w * 0.90f), MIN_W, 1600);
        win_h = std::clamp(static_cast<int>(usable.h * 0.88f), MIN_H, 1100);
    }

    SDL_Window* window = SDL_CreateWindow(
        "Athena",
        win_w,
        win_h,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
    );
    if (!window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowMinimumSize(window, MIN_W, MIN_H);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Position pos;
    pos.init(STARTPOS);

    std::vector<UndoState> undo_stack;
    undo_stack.reserve(256);

    std::vector<std::string> move_history;
    move_history.reserve(512);
    std::array<bool, COLOR_NB> eliminated{};
    eliminated.fill(false);

    Square selected = Square::Offboard;
    Square last_from = Square::Offboard;
    Square last_to = Square::Offboard;
    std::string status = "Ready. Select a piece, then a destination.";

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                {
                    running = false;
                }
                else if (event.key.key == SDLK_R)
                {
                    pos.init(STARTPOS);
                    undo_stack.clear();
                    move_history.clear();
                    eliminated.fill(false);
                    selected = Square::Offboard;
                    last_from = Square::Offboard;
                    last_to = Square::Offboard;
                    status = "Position reset.";
                }
                else if (event.key.key == SDLK_U)
                {
                    if (!undo_stack.empty())
                    {
                        pos = undo_stack.back().pos;
                        eliminated = undo_stack.back().eliminated;
                        undo_stack.pop_back();
                        if (!move_history.empty()) move_history.pop_back();
                        selected = Square::Offboard;
                        last_from = Square::Offboard;
                        last_to = Square::Offboard;
                        status = "Undid last move.";
                    }
                    else
                    {
                        status = "No moves to undo.";
                    }
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                const RenderMetrics rm = query_render_metrics(window, renderer);
                const Layout l = compute_layout(rm.render_w, rm.render_h);
                const float mouse_x = event.button.x * rm.scale_x;
                const float mouse_y = event.button.y * rm.scale_y;
                const SDL_FRect reset_btn = reset_button_rect(l);

                if (point_in_rect(mouse_x, mouse_y, reset_btn))
                {
                    pos.init(STARTPOS);
                    undo_stack.clear();
                    move_history.clear();
                    eliminated.fill(false);
                    selected = Square::Offboard;
                    last_from = Square::Offboard;
                    last_to = Square::Offboard;
                    status = "Position reset.";
                    continue;
                }

                Square clicked;
                if (!square_from_mouse(l, mouse_x, mouse_y, clicked))
                {
                    selected = Square::Offboard;
                    continue;
                }

                if (selected == Square::Offboard)
                {
                    const PieceClass pc = pos.board(clicked);
                    if (pc != PieceClass::Empty() && pc.color() == pos.turn() && !eliminated[static_cast<uint8_t>(pc.color())])
                    {
                        selected = clicked;
                        status = std::string("Selected ") + clicked.uci();
                    }
                    else
                    {
                        status = "Select a piece for the side to move.";
                    }
                }
                else
                {
                    if (clicked == selected)
                    {
                        selected = Square::Offboard;
                        status = "Selection cleared.";
                        continue;
                    }

                    const std::string uci = build_uci(selected, clicked, pos);
                    Move move;
                    std::string error;

                    if (parse_move_uci(pos, uci, move, &error))
                    {
                        undo_stack.push_back(UndoState{pos, eliminated});
                        pos.makemove(move);
                        move_history.push_back(uci);
                        last_from = selected;
                        last_to = clicked;
                        status = "Move played: " + uci;
                        const std::string advance_msg = apply_checkmates_and_skip(pos, eliminated);
                        if (!advance_msg.empty()) status += " " + advance_msg;
                    }
                    else
                    {
                        status = error;
                    }

                    selected = Square::Offboard;
                }
            }
        }

        const RenderMetrics rm = query_render_metrics(window, renderer);
        const Layout l = compute_layout(rm.render_w, rm.render_h);

        render_scene(renderer, pos, l, selected, last_from, last_to, status, move_history, eliminated);

        std::string title = std::string("Athena | win ")
            + std::to_string(rm.window_w) + "x" + std::to_string(rm.window_h)
            + " | render " + std::to_string(rm.render_w) + "x" + std::to_string(rm.render_h);
        SDL_SetWindowTitle(window, title.c_str());
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
