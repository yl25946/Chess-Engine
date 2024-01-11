import chess
import copy
import time as stopwatch
import pesto_eval as pesto


def search(board, turn, time):
    """
    Uses alpha-beta pruning with negamax to find the best move in this position

    Uses iterative deepening

    TODO: If searching with depth <= 1, errors out
    TODO: find a way to not return None when time is too short

    LITERALLY JUST AN ABSTRACTION OF ALPHA_BETA_NEGAMAX

    Parameters:
    board (chess.Board()): the current board
    turn (chess.WHITE or chess.BLACK): color you want to find the best move
    time (int): number of seconds to calculate a move

    Return:
    chess.Move: the best move in that position
    int: the evaluation of the fed into board
    """
    depthCounter = 1
    end_time = stopwatch.time() + time
    last_completed_search = None

    # implements iterative deepening
    try:
        while (True):
            last_completed_search = alpha_beta_negamax_search(
                board, turn, depthCounter, -100000, 100000, end_time)
            depthCounter += 1
    # time is up
    except Exception as e:
        print("Searched Depth(half-moves): " + str(depthCounter - 1))
        return last_completed_search


def alpha_beta_negamax_search(board, turn, depth, alpha, beta, end_time):
    """
    Uses alpha-beta pruning with negamax to find the best move in this position

    Parameters:
    board (chess.Board()): the current board
    turn (chess.WHITE or chess.BLACK): color you want to find the best move
    depth (int): number of half-moves you want to search
    alpha (int): worst you can get
    beta (int): the best your opponent can get
    end_time (long): time we want to finish calculations by

    Return:
    chess.Move: the best move in that position
    int: the evaluation of the fed into board

    TODO: Add in null-move heuristic
    TODO: add queistence search
    TODO: Add transposition table
    """

    # check if time is up, then throw an exception
    if (stopwatch.time() >= end_time):
        raise Exception("Out of time!")

    # if we ever check for a move here we're FUCKED

    # checking end-states
    if (board.is_checkmate()):
        return None, -10000
    if (board.is_stalemate()):
        return None, 0
    if (board.is_insufficient_material()):
        return None, 0
    if (board.is_fifty_moves()):
        return None, 0
    # TODO: ADD IN EFFICIENT REPITITION

    # return heuristic evaluation if the game hasn't ended
    # TODO: ADD QUIESCE HERE
    if (depth == 0):
        # print(board.fen())
        # print(pesto.eval(board, turn))
        return None, pesto.eval(board, turn)

    # start the search
    legal_moves = board.legal_moves
    # best_eval = -10000
    best_move = None

    # TODO: ADD GOOD MOVE ORDERING
    # curr_move is the current move we are searching
    for curr_move in legal_moves:
        # make a copy of the board and search it
        board_copy = copy.deepcopy(board)
        board_copy.push(curr_move)
        # turn ^ 1 flips the turn
        search_result = alpha_beta_negamax_search(
            board_copy, turn ^ 1, depth - 1, -beta, -alpha, end_time)
        eval = - search_result[1]
        if (eval >= beta):
            # uh if it does this on the first move we're kinda fucked
            # but if it returns we never use it?
            return curr_move, beta
        if (eval > alpha):
            # alpha acts like max in MiniMax
            alpha = eval
            best_move = curr_move

    return best_move, alpha


# def negamax(board, turn, depth, end_time):
#     """
#     Uses alpha-beta pruning with negamax to find the best move in this position

#     Parameters:
#     board (chess.Board()): the current board
#     turn (chess.WHITE or chess.BLACK): color you want to find the best move
#     depth (int): number of half-moves you want to search
#     alpha (int): worst you can get
#     beta (int): the best your opponent can get
#     end_time (long): time we want to finish calculations by

#     Return:
#     chess.Move: the best move in that position
#     int: the evaluation of the fed into board

#     TODO: Add in null-move heuristic
#     TODO: add queistence search
#     TODO: Add transposition table
#     """

#     # check if time is up, then throw an exception
#     if (stopwatch.time() >= end_time):
#         print(end_time)
#         print(stopwatch.time())
#         raise Exception("Out of time!")

#     # if we ever check for a move here we're FUCKED

#     # checking end-states
#     if (board.is_checkmate()):
#         return None, -10000
#     if (board.is_stalemate()):
#         return None, 0
#     if (board.is_insufficient_material()):
#         return None, 0
#     if (board.is_fifty_moves()):
#         return None, 0
#     # TODO: ADD IN EFFICIENT REPITITION

#     # return heuristic evaluation if the game hasn't ended
#     # TODO: ADD QUIESCE HERE
#     if (depth == 0):
#         # print(board.fen())
#         # print(pesto.eval(board, turn))
#         return None, pesto.eval(board, turn)

#     # start the search
#     legal_moves = board.legal_moves
#     best_eval = -10000
#     best_move = None

#     # TODO: ADD GOOD MOVE ORDERING
#     # curr_move is the current move we are searching
#     for curr_move in legal_moves:
#         # make a copy of the board and search it
#         board_copy = copy.deepcopy(board)
#         board_copy.push(curr_move)
#         # turn ^ 1 flips the turn
#         search_result = negamax(
#             board_copy, turn ^ 1, depth - 1, end_time)
#         if (search_result[1] > best_eval):
#             best_move = search_result[0]
#             best_eval = search_result[1]
#     return best_move, best_eval


pesto.init_tables()
board = chess.Board()
board.set_fen(
    "3qr1k1/p5pp/2pb1n2/p2p4/P7/1PNP3P/3Q1PP1/2B1R1K1 b - - 0 21")
print(board)
print(search(board, chess.BLACK, 15600))
