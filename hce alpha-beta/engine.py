import chess
import search
import pesto_eval as pesto

input = input("0 if you want engine to be white, 1 if black")

# sets the color the engine will be playing as
engine_color = chess.WHITE if input == 0 else chess.BLACK


