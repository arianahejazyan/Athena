import sys
from PyQt6.QtWidgets import QApplication

from parsers import *
from interface.CLI import *
from interface.GUI.windows import *

class Talia:

    def __init__(self):

        self.app = QApplication(sys.argv)


        self.terminal_window = TerminalWindow(self)
        self.board_window = BoardWindow(self)
        self.edit_window = EditWindow(self)
        self.analyze_window = AnalyzeWindow(self)

        self.interpreter = Interpreter(self)
        self.piece_parser = PieceParser(self)
        self.fen_parser = FENParser(self)
        self.pgn_parser = PGNParser(self)
        

    def start(self):

        
        self.terminal_window.show()
        sys.exit(self.app.exec())

if __name__ == '__main__':

    program = Talia()
    program.start()