#!/usr/bin/env python

import random
from string import ascii_lowercase


HANGMAN_FRAMES = ['''
  +---+
      |
      |
      |
      |
      |
=========''', '''
  +---+
  |   |
      |
      |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
      |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
  |   |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|   |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|\  |
      |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|\  |
 /    |
      |
=========''', '''
  +---+
  |   |
  O   |
 /|\  |
 / \  |
      |
=========''']

MAX_INCORRECT_GUESSES = len(HANGMAN_FRAMES) - 1 # first frame does not count towards guess count

TITLE = '''                                        
| |                                            
| |__   __ _ _ __   __ _ _ __ ___   __ _ _ __  
| '_ \ / _` | '_ \ / _` | '_ ` _ \ / _` | '_ \ 
| | | | (_| | | | | (_| | | | | | | (_| | | | |
|_| |_|\__,_|_| |_|\__, |_| |_| |_|\__,_|_| |_|
                    __/ |                      
                   |___/                       
'''


def get_words_from_file(filename):
    '''Read words in from file f, return list of words'''

    words = []
    with open(filename, 'r') as f:
        for line in f:  # assumes each word is on its own line
            words.append(line.strip())
    
    return words


def draw_frame(i):
    '''Draws hangman frame based on number of incorrect guesses'''

    return HANGMAN_FRAMES[i]


def draw_hangman_word(word, guessed_letters):
    '''Draws hangman word, not printing letters that have not been guessed'''

    out = []
    for letter in word:
        char = letter if letter in guessed_letters else '_'
        out.append(char)
    return ''.join(out)


def draw_guessed_letters(guessed_letters):
    '''Prints letters that have already been guessed'''

    out = list("Letters already guessed: ") # list is easier to work with than string
    for letter in ascii_lowercase:
        char = letter if letter in guessed_letters else '_'
        out.append(char)
    
    return ''.join(out)



def main():
    print(TITLE)

    words = get_words_from_file('words.txt')
    word = random.choice(words)
    # print("word = {}".format(word))

    num_incorrect_guesses = 0
    player_wins = False
    guessed_letters = set()

    while num_incorrect_guesses < MAX_INCORRECT_GUESSES:

        out = ''  # string that holds content to be printed out
        num_lines = 0  # used to moved cursor back up, to draw over previous content
        out += draw_frame(num_incorrect_guesses) + '\n'
        out += '\n'
        out += draw_guessed_letters(guessed_letters) + '\n'
        out += '\n'
        out += draw_hangman_word(word, guessed_letters) + '\n'
        num_lines = out.count('\n') + 1
        print(out, end='')

        guess = input('Guess a letter: ')
        guessed_letters.add(guess)
        num_lines += 1  # newline added when player enters character

        if guess not in word:
            num_incorrect_guesses += 1

        if set(word) <= guessed_letters: # if each letter in word has been guessed
            player_wins = True
            break
        
        # Move cursor back up
        print('\r' + '\033[1A' * num_lines)
    
    if num_incorrect_guesses == MAX_INCORRECT_GUESSES:
        print(draw_frame(MAX_INCORRECT_GUESSES))
        print('\n' * (num_lines - draw_frame(MAX_INCORRECT_GUESSES).count('\n') - 1))
        print('You lose!')
    
    if player_wins:
        print(word)
        print('You win!')
        

main()
