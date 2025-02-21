import json

class StyleManager:
    @staticmethod
    def load(file):
        with open(file, 'r') as f:
            return json.load(f)