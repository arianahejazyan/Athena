#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <array>
#include <string>
#include "chess.h"
#include "move.h"

using namespace std;

namespace ELSA
{

template<typename T>
struct Node
{
    T data;
    Node* prev;
    Node* next;
};

template<typename T>
struct List
{
    private:

        Node<T>* head;

    public:
        
        List(): head(new Node<T>()) {}

        void rollback() {
            while (head->prev) head = head->prev;
        }

        T& node() const {
            return head->data;
        }

        void clone()
        {
            const Node<T>* old = head;

            if (head->next == nullptr)
            {
                Node<T>* node = new Node<T>();
                node->prev = head;
                head->next = node;
                head = node;
            }

            else head = head->next;

            head->data.clone(old->data);
        }

        void restore() {
            head = head->prev;
        }  
};

struct Info
{
    u64 hash;
    CastlingRights castle;
    Piece captured;
    Player turn;
    Square epsq;
    int halfmove;
    int fullmove;

    void clone(const Info& other);
};

struct GameState
{
    private:

        array<Piece, SQUARE_NB> mailbox;
        array<u64, PLAYER_NB> occupied;
        array<u64, PIECE_TYPE_NB> bitboard;
        
        List<Info> info;

    public:

        Info& stateInfo() const;

        void resetGameState();

        Piece at(Square sq) const;

        u64 bb(const PieceType type) const;

        u64 occ(const Player player) const;

        void setPiece(Square sq, Piece piece);

        void popPiece(Square sq);

        void makemove(const Move move);

        void undomove(const Move move);
};

}; // namespace

#endif