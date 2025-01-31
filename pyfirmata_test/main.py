import pyfirmata2
import time

if __name__ == '__main__':
    board = pyfirmata2.Arduino('COM5')
    print('Communication successfully started')

    while True:
        board.digital[13].write(1)
        time.sleep(1)
        board.digital[13].write(0)
        time.sleep(1)
