import chess
import copy
import pesto_eval as pesto

def search(board, turn, depth):
    """
    Uses alpha-beta pruning with negamax to find the best move in this position

    LITERALLY JUST AN ABSTRACTION OF ALPHA_BETA_NEGAMAX

    Parameters:
    board (chess.Board()): the current board
    turn (chess.WHITE or chess.BLACK): color you want to find the best move
    depth (int): number of half-moves you want to search

    Return:
    chess.Move: the best move in that position
    int: the evaluation of the fed into board
    """
    return alpha_beta_negamax_search(board, turn, depth, -100000, 100000)
    

def alpha_beta_negamax_search(board, turn, depth, alpha, beta):
    """
    Uses alpha-beta pruning with negamax to find the best move in this position

    Parameters:
    board (chess.Board()): the current board
    turn (chess.WHITE or chess.BLACK): color you want to find the best move
    depth (int): number of half-moves you want to search
    alpha (int): worst you can get
    beta (int): the best your opponent can get

    Return:
    chess.Move: the best move in that position
    int: the evaluation of the fed into board

    TODO: Add in null-move heuristic
    TODO: add queistence search
    """
    # checking end-states
    if(board.is_checkmate()): return -10000
    if(board.is_stalemate()): return 0
    if(board.is_insufficient_material()): return 0

    # return heuristic evaluation if the game hasn't ended
    # TODO: ADD QUIESCE HERE
    if(depth == 0): return pesto.eval(board, turn)

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
        # negamax search with alpha-beta
        eval =  - search(board_copy, depth - 1, -beta, -alpha)
        if(eval <= beta):
            # uh if it does this on the first move we're kinda fucked
            # but if it returns we never use it?
            return curr_move, beta
        if(eval > alpha):
            # alpha acts like max in MiniMax
            alpha = eval
            best_move = curr_move

    return best_move, alpha            
        
    

