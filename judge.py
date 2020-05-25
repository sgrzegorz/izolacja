import os
import sys

OK = 0
WRONG_ARGUMENTS = 1
WRONG_INPUT = 2
WRONG_OUTPUT = 3
UNEXPECTED_ERROR = 20
PEOPLE_TOO_CLOSE = 42
NOT_ON_ROAD = 66
NON_UNIQUE = 96


def check_input_file(header, board):
    if len(header.split()) != 4:
        exit_with(WRONG_INPUT, 'First line must contain 4 numbers')
    for x in header.split():
        if not x.isnumeric():
            exit_with(WRONG_INPUT, 'First line must contain 4 numbers')

    width, height, distance, population = [int(x) for x in header.split()]
    if len(board) != height:
        exit_with(WRONG_INPUT, 'Board has wrong dimensions')
    for row in board:
        if len(row) != width:
            exit_with(WRONG_INPUT, 'Board has wrong dimensions')
        for cell in row:
            if cell not in ['.', '-', '|', '+']:
                exit_with(WRONG_INPUT, 'Board must contain only . - | and + characters')
    return True


def check_output_file(output, expected_population, board_width, board_height):
    if len(output) < expected_population:
        exit_with(WRONG_OUTPUT, 'Output file must contain %s lines (as defined in input file)' % expected_population)
    for line in output:
        if len(line) != 2 or not line[0].isnumeric() or not line[1].isnumeric:
            exit_with(WRONG_OUTPUT, 'Each line of output file must contain exactly 2 numbers')
        # for coord in line:
        #     coord = int(coord)
        #     if coord < 0 or coord > board_height:
        #         exit_with(WRONG_OUTPUT, 'Coordinates (%s, %s) are outside the board' % tuple(line))

def read_board(filename):
    with open(filename) as board_file:
        raw_board = board_file.read()
        raw_board = [x for x in raw_board.split('\n') if x]
        header = raw_board[0]
        board = raw_board[2:]
        check_input_file(header, board)
        width, height, distance, population = [int(x) for x in header.split()]
        return distance, population, board, width, height


def read_result(filename, population, width, height):
    with open(filename) as result_file:
        raw_result = [x.split() for x in result_file.read().split('\n') if x]
        check_output_file(raw_result, population, width, height)
        result = [(int(y), int(x)) for (x, y) in raw_result]
        return result


def neighbours(board, i, j, width, height):
    neighbours = []
    if board[i][j] in ('|', '+'):
        if i > 0 and board[i-1][j] in ('|', '+'):
            neighbours.append((i-1, j))
        if i < height-1 and board[i+1][j] in ('|', '+'):
            neighbours.append((i+1, j))
    if board[i][j] in ('-', '+'):
        if j > 0 and board[i][j-1] in ('-', '+'):
            neighbours.append((i, j-1))
        if j < width-1 and board[i][j+1] in ('-', '+'):
            neighbours.append((i, j+1))
    return neighbours


def check_distances(board, population, distance, width, height):
    distances = [[-1 for i in range(width)] for j in range(height)]

    for i in range(len(population)):
        x, y = population[i]
        distances[x][y] = i
        queue = []
        for a, b in neighbours(board, x, y, width, height):
            queue.append((a, b, 1))

        while queue:
            x, y, d = queue.pop(0)
            if d <= distance and distances[x][y] < i:
                if (x, y) in population:
                    exit_with(PEOPLE_TOO_CLOSE, 'People are too close to each other')
                distances[x][y] = i
                for (a, b) in neighbours(board, x, y, width, height):
                    queue.append((a, b, d+1))

    return OK

def check_placement( board, result ):
    for (y,x) in result:
        if board[y][x] == ".":
            exit_with(NOT_ON_ROAD,"Person not on a road")

    unique = set()
    for (y,x) in result:
        if (x,y) in unique:
            exit_with(NON_UNIQUE,"Two people on the same position")
        unique.add((x,y))

    return OK




def check_args():
    if len(sys.argv) < 3 or len(sys.argv) > 4:
        exit_with(WRONG_ARGUMENTS, 'Judge expects 2 or 3 arguments')
    if sys.argv[1] not in os.listdir():
        exit_with(WRONG_ARGUMENTS, 'Input file does not exist')
    if sys.argv[2] not in os.listdir():
        exit_with(WRONG_ARGUMENTS, 'Output file does not exist')


def exit_with(code, message):
    print(message)
    exit(code)


if __name__ == '__main__':
    try:
        check_args()
        board_file_name = sys.argv[1]
        result_file_name = sys.argv[2]
        distance, population, board, width, height = read_board(board_file_name)
        result = read_result(result_file_name, population, width, height)
        #        for i in range(len(board)): print(board[i])
        #        print(result)
        check_placement(board, result)

        check_distances(board, result, distance, width, height)
        exit_with(OK, 'OK')
    except Exception as e:
        exit_with(UNEXPECTED_ERROR, '%s' % e)