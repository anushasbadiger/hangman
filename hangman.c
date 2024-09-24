#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 50
#define MAX_TRIES 6

// Struct to hold a word and its hint
struct WordWithHint {
    char word[MAX_WORD_LENGTH];
    char hint[MAX_WORD_LENGTH];
};

// Function declarations
void displayWord(const char guessedWord[]);
void drawHangman(int tries);
void selectCategory(struct WordWithHint* selectedWord);
void playGame();

int main() {
    char playAgain = 'y';
    
    printf("Welcome to the Enhanced Hangman Game!\n");
    
    // Main game loop to allow replay
    while (playAgain == 'y' || playAgain == 'Y') {
        playGame();
        
        printf("\nWould you like to play again? (y/n): ");
        scanf(" %c", &playAgain);
    }

    printf("Thank you for playing Hangman! Goodbye!\n");
    return 0;
}

void playGame() {
    // Seed the random number generator with the current time
    srand(time(NULL));
    
    // Select a word category and a word
    struct WordWithHint selectedWord;
    selectCategory(&selectedWord);

    const char* secretWord = selectedWord.word;
    const char* hint = selectedWord.hint;

    int wordLength = strlen(secretWord);
    char guessedWord[MAX_WORD_LENGTH] = { 0 };
    
    // Initialize guessedWord with underscores
    for (int i = 0; i < wordLength; i++) {
        guessedWord[i] = '_';
    }
    guessedWord[wordLength] = '\0';  // Null-terminate

    bool guessedLetters[26] = { false };  // To track guessed letters
    char incorrectGuesses[MAX_TRIES + 1] = { 0 };  // To store wrong guesses
    int tries = 0;
    
    printf("Hint: %s\n", hint);
    
    // Main game loop
    while (tries < MAX_TRIES) {
        printf("\n");
        displayWord(guessedWord);
        drawHangman(tries);
        
        // Show incorrect guesses
        if (tries > 0) {
            printf("Incorrect guesses: %s\n", incorrectGuesses);
        }
        
        char guess;
        printf("Enter a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);
        
        if (guess < 'a' || guess > 'z') {
            printf("Invalid input. Please enter a valid letter.\n");
            continue;
        }

        if (guessedLetters[guess - 'a']) {
            printf("You've already guessed that letter. Try again.\n");
            continue;
        }
        
        guessedLetters[guess - 'a'] = true;

        bool found = false;
        for (int i = 0; i < wordLength; i++) {
            if (secretWord[i] == guess) {
                guessedWord[i] = guess;  // Update guessedWord with correct guess
                found = true;
            }
        }

        if (found) {
            printf("Good guess!\n");
        } else {
            printf("Sorry, the letter '%c' is not in the word.\n", guess);
            incorrectGuesses[tries] = guess;  // Track wrong guesses
            tries++;
        }

        if (strcmp(secretWord, guessedWord) == 0) {
            printf("\nCongratulations! You've guessed the word: %s\n", secretWord);
            return;  // Game won, exit loop
        }
    }

    // If max tries exceeded, show failure message
    if (tries >= MAX_TRIES) {
        drawHangman(tries);
        printf("\nSorry, you've run out of tries. The word was: %s\n", secretWord);
    }
}

// Function to display the current state of the word
void displayWord(const char guessedWord[]) {
    printf("Word: ");
    for (int i = 0; guessedWord[i] != '\0'; i++) {
        printf("%c ", guessedWord[i]);
    }
    printf("\n");
}

// Function to draw the hangman
void drawHangman(int tries) {
    const char* hangmanParts[] = {
        "     _________",
        "    |         |",
        "    |         O",
        "    |        /|\\",
        "    |        / \\",
        "    |"
    };

    printf("\n");
    for (int i = 0; i <= tries && i < 6; i++) {
        printf("%s\n", hangmanParts[i]);
    }
}

// Function to allow the user to select a word category
void selectCategory(struct WordWithHint* selectedWord) {
    struct WordWithHint animals[] = {
        { "elephant", "A large mammal with a trunk" },
        { "giraffe", "Tall animal with a long neck" },
        { "dolphin", "An intelligent sea mammal" }
    };

    struct WordWithHint food[] = {
        { "pizza", "A popular Italian dish" },
        { "burger", "A sandwich with a meat patty" },
        { "sushi", "A Japanese dish with rice and fish" }
    };

    struct WordWithHint places[] = {
        { "beach", "Sandy shore by the sea" },
        { "mountain", "A tall natural elevation" },
        { "desert", "A dry, sandy region" }
    };

    int categoryChoice;
    printf("\nSelect a category:\n");
    printf("1. Animals\n");
    printf("2. Food\n");
    printf("3. Places\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &categoryChoice);

    struct WordWithHint* category;
    int categorySize;

    switch (categoryChoice) {
        case 1:
            category = animals;
            categorySize = sizeof(animals) / sizeof(animals[0]);
            break;
        case 2:
            category = food;
            categorySize = sizeof(food) / sizeof(food[0]);
            break;
        case 3:
            category = places;
            categorySize = sizeof(places) / sizeof(places[0]);
            break;
        default:
            printf("Invalid choice. Defaulting to Animals.\n");
            category = animals;
            categorySize = sizeof(animals) / sizeof(animals[0]);
            break;
    }

    // Randomly select a word from the chosen category
    int wordIndex = rand() % categorySize;
    *selectedWord = category[wordIndex];
}