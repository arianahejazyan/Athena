from PyQt6.QtGui import QFont, QFontDatabase

class FontManager:
    @staticmethod
    def load(file):
        return QFont(QFontDatabase.applicationFontFamilies(QFontDatabase.addApplicationFont(file))[0])