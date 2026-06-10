/*
Console-Based Quiz Game
Description: This is a simple quiz game that starts with a main menu for playing,
viewing scores, or exiting. Players enter their name, choose a category and the
level of difficulty, then answer 10 multiple-choice questions.
Author: [25F-0594 Sana Irfan, 25F-0627 Dania Mustafa, 25F-0690 Hijab Fatima]
Course: [CL1002 - Programming Fundamentals Lab]
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Global variables

string playerName;
int playerScore = 0;
int correctAnswers = 0;
int wrongAnswers = 0;
int currentDifficulty = 1; // 1=Easy, 2=Medium, 3=Hard
string currentCategory = "";
// Parallel arrays to store question data (max 100 questions)
string questionText[100];
string optionA[100];
string optionB[100];
string optionC[100];
string optionD[100];
int correctAnswer[100];
bool answeredWrong[100];
// Arrays for wrong questions review
string wrongQuestionText[10];
string wrongOptionA[10];
string wrongOptionB[10];
string wrongOptionC[10];
string wrongOptionD[10];
int wrongCorrectAnswer[10];
int totalQuestionsLoaded = 0;
int wrongQuestionCount = 0;
int lifelinesLeft = 2; // 50/50 and Extra Time lifelines
bool extraTimeUsed = false;
int correctStreak = 0;
const int question_time = 10; // 10 seconds per question
bool timeUp = false;

// Function declarations

void showMainMenu(); // Displays the main menu with game options
void startNewQuiz(); // Initiates a new quiz with player setup
void showHighScores(); // Shows the top scores from the score file
void loadQuestions(string filename); // Loads questions from a specified file into memory
void askQuestions(); // Manages the flow of asking all quiz questions
bool askSingleQuestion(int questionIndex, int questionNum); // Presents one question with timer and scoring
void savePlayerScore(); // Saves current player's score to the high score file
void displayFinalResults(); // Shows the player's final score and performance
void showCategoryMenu(); // Displays available quiz categories to choose from
void showDifficultyMenu(); // Shows difficulty levels with scoring information
void applyLifeline(int questionIndex); // Applies 50/50 lifeline by hiding wrong options
void clearInputBuffer(); // Clear input buffer
void shuffleQuestions(); // Randomizes the order of loaded questions
void shuffleOptions(int questionIndex); // Randomizes the order of options for a question
void reviewWrongAnswers(); // Displays questions that were answered incorrectly
void replayQuiz(); // Allows replaying the last quiz with same questions
// Timer functions
bool checkTimeExpired(int startTime, int timeLimit); // Checks if time has run out for a question
void displayTimer(int remainingTime); // Shows the countdown timer during question answering

// Main function for execution of console game
int main() 
{
    srand(time(0));
    cout << "=============================================\n";
    cout << "     CONSOLE-BASED QUIZ GAME\n";
    cout << "=============================================\n";
    int choice;
    bool exit = false;
    while (!exit) 
    {
        showMainMenu();
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        clearInputBuffer();
        switch (choice) 
        {
        case 1:
            startNewQuiz();
            break;
        case 2:
            showHighScores();
            break;
        case 3:
            replayQuiz();
            break;
        case 4:
            cout << "\nThank you for playing! Goodbye!\n";
            exit = true;
            break;
        default:
            cout << "\nInvalid choice! Please try again.\n";
        }
        if (!exit) 
        {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    }
    system("pause");
    return 0;
}

// Function to clear input buffer
void clearInputBuffer() 
{
    cin.clear();
    cin.ignore(1000, '\n');
}

// Function to show main menu
void showMainMenu() 
{
    cout << endl << "========== MAIN MENU ==========\n";
    cout << "1. Start New Quiz\n";
    cout << "2. View High Scores\n";
    cout << "3. Replay Last Quiz\n";
    cout << "4. Exit Game\n";
    cout << "==============================\n";
}

// Function to start a new quiz
void startNewQuiz() 
{
    // Reset scores and streak
    playerScore = 0;
    correctAnswers = 0;
    wrongAnswers = 0;
    correctStreak = 0;
    lifelinesLeft = 2;
    extraTimeUsed = false;
    wrongQuestionCount = 0;
    timeUp = false;
    cout << endl << "Enter your name: ";
    getline(cin, playerName);
    showCategoryMenu();
    int categoryChoice;
    cout << "Enter category (1-5): ";
    cin >> categoryChoice;
    clearInputBuffer();
    string filename;
    switch (categoryChoice) 
    {
    case 1:
        filename = "science.txt";
        currentCategory = "Science";
        break;
    case 2:
        filename = "computer.txt";
        currentCategory = "Computer";
        break;
    case 3:
        filename = "sports.txt";
        currentCategory = "Sports";
        break;
    case 4:
        filename = "history.txt";
        currentCategory = "History";
        break;
    case 5:
        filename = "IQ.txt";
        currentCategory = "IQ/Logic";
        break;
    default:
        cout << "Invalid choice! Using Science category.\n";
        filename = "science.txt";
        currentCategory = "Science";
    }
    // Choose difficulty
    showDifficultyMenu();
    cout << "Enter difficulty (1-3): ";
    cin >> currentDifficulty;
    clearInputBuffer();
    if (currentDifficulty < 1 || currentDifficulty > 3) 
    {
        cout << "Invalid choice! Using Medium difficulty.\n";
        currentDifficulty = 2;
    }
    // Load questions from file
    loadQuestions(filename);
    if (totalQuestionsLoaded == 0) 
    {
        cout << "No questions loaded. Quiz cannot start.\n";
        return;
    }
    // Shuffle questions for randomness
    shuffleQuestions();
    // Display quiz start message
    cout << "\n=============================================\n";
    cout << "   QUIZ STARTED - " << currentCategory << " Category\n";
    cout << "   Difficulty: ";
    if (currentDifficulty == 1) cout << "Easy";
    else if (currentDifficulty == 2) cout << "Medium";
    else cout << "Hard";
    cout << "\n   Time per question: " << question_time << " seconds\n";
    cout << "=============================================\n\n";
    askQuestions();
    displayFinalResults();
    savePlayerScore();
    // Ask if player wants to review wrong answers
    if (wrongQuestionCount > 0) 
    {
        cout << "\nDo you want to review wrong answers? (y/n): ";
        char reviewChoice;
        cin >> reviewChoice;
        clearInputBuffer();
        if (reviewChoice == 'y' || reviewChoice == 'Y') 
        {
            reviewWrongAnswers();
        }
    }
}

// Function to show category menu
void showCategoryMenu() 
{
    cout << endl << "========== SELECT CATEGORY ==========\n";
    cout << "1. Science\n";
    cout << "2. Computer\n";
    cout << "3. Sports\n";
    cout << "4. History\n";
    cout << "5. IQ/Logic\n";
    cout << "====================================\n";
}

// Function to show difficulty menu
void showDifficultyMenu() 
{
    cout << "\n========== SELECT DIFFICULTY ==========\n";
    cout << "1. Easy (+2/-2 points)\n";
    cout << "2. Medium (+3/-3 points)\n";
    cout << "3. Hard (+5/-5 points)\n";
    cout << "======================================\n";
}

// Function to load questions from file
void loadQuestions(string filename) 
{
    ifstream questionFile(filename.c_str());
    if (!questionFile.is_open()) {
        cout << "Error: Could not open " << filename << " file.\n";
        totalQuestionsLoaded = 0;
        return;
    }
    string line;
    totalQuestionsLoaded = 0;
    while (totalQuestionsLoaded < 100 && getline(questionFile, line)) {
        // Read question text
        questionText[totalQuestionsLoaded] = line;
        answeredWrong[totalQuestionsLoaded] = false;
        // Read 4 options
        if (!getline(questionFile, optionA[totalQuestionsLoaded])) break;
        if (!getline(questionFile, optionB[totalQuestionsLoaded])) break;
        if (!getline(questionFile, optionC[totalQuestionsLoaded])) break;
        if (!getline(questionFile, optionD[totalQuestionsLoaded])) break;
        // Read correct answer
        string answerStr;
        if (!getline(questionFile, answerStr)) 
        {
            totalQuestionsLoaded = 0;
            questionFile.close();
            return;
        }
        correctAnswer[totalQuestionsLoaded] = atoi(answerStr.c_str());
        totalQuestionsLoaded++;
        // Skip empty line if exists
        getline(questionFile, line);
    }
    questionFile.close();
    if (totalQuestionsLoaded == 0) 
        cout << "No questions found in the file.\n";
    else 
        cout << "Loaded " << totalQuestionsLoaded << " questions from " << filename << "\n";
}

// Function to shuffle questions using Fisher-Yates algorithm
void shuffleQuestions() 
{
    if (totalQuestionsLoaded <= 1)
        return;
    for (int i = totalQuestionsLoaded - 1; i > 0; i--) 
    {
        int j = rand() % (i + 1);
        // Swap all question data
        // Swap question text
        string tempText = questionText[i];
        questionText[i] = questionText[j];
        questionText[j] = tempText;
        // Swap options
        string tempA = optionA[i];
        optionA[i] = optionA[j];
        optionA[j] = tempA;
        string tempB = optionB[i];
        optionB[i] = optionB[j];
        optionB[j] = tempB;
        string tempC = optionC[i];
        optionC[i] = optionC[j];
        optionC[j] = tempC;
        string tempD = optionD[i];
        optionD[i] = optionD[j];
        optionD[j] = tempD;
        // Swap correct answer
        int tempAns = correctAnswer[i];
        correctAnswer[i] = correctAnswer[j];
        correctAnswer[j] = tempAns;
        // Swap answeredWrong flag
        bool tempWrong = answeredWrong[i];
        answeredWrong[i] = answeredWrong[j];
        answeredWrong[j] = tempWrong;
    }
}

// Function to shuffle options for a question
void shuffleOptions(int questionIndex) 
{
    // Store original options
    string originalOptions[4];
    originalOptions[0] = optionA[questionIndex];
    originalOptions[1] = optionB[questionIndex];
    originalOptions[2] = optionC[questionIndex];
    originalOptions[3] = optionD[questionIndex];
    // Store original correct answer
    int originalCorrect = correctAnswer[questionIndex];
    string correctOption = originalOptions[originalCorrect - 1];
    // Create an array for new order
    int newOrder[4] = { 0, 1, 2, 3 };
    // Shuffle the order
    for (int i = 3; i > 0; i--) 
    {
        int j = rand() % (i + 1);
        // Swap positions
        int temp = newOrder[i];
        newOrder[i] = newOrder[j];
        newOrder[j] = temp;
        // Update correct answer position
        if (i + 1 == originalCorrect)
            originalCorrect = j + 1;
        else if (j + 1 == originalCorrect)
            originalCorrect = i + 1;
    }
    // Apply new order to options
    optionA[questionIndex] = originalOptions[newOrder[0]];
    optionB[questionIndex] = originalOptions[newOrder[1]];
    optionC[questionIndex] = originalOptions[newOrder[2]];
    optionD[questionIndex] = originalOptions[newOrder[3]];
    // Update correct answer
    correctAnswer[questionIndex] = originalCorrect;
}

// Function to ask all questions
void askQuestions() 
{
    int questionsToAsk = (totalQuestionsLoaded < 10) ? totalQuestionsLoaded : 10;
    for (int i = 0; i < questionsToAsk; i++) 
    {
        // Create temporary copies of options for this question
        string tempA = optionA[i];
        string tempB = optionB[i];
        string tempC = optionC[i];
        string tempD = optionD[i];
        int tempCorrect = correctAnswer[i];
        // Shuffle options for this question
        shuffleOptions(i);
        // Ask the question with timer
        bool answeredCorrectly = askSingleQuestion(i, i + 1);
        // Store if answered wrong
        if (!answeredCorrectly && wrongQuestionCount < 10) 
        {
            wrongQuestionText[wrongQuestionCount] = questionText[i];
            wrongOptionA[wrongQuestionCount] = optionA[i];
            wrongOptionB[wrongQuestionCount] = optionB[i];
            wrongOptionC[wrongQuestionCount] = optionC[i];
            wrongOptionD[wrongQuestionCount] = optionD[i];
            wrongCorrectAnswer[wrongQuestionCount] = correctAnswer[i];
            wrongQuestionCount++;
        }
        // Restore original options for future use
        optionA[i] = tempA;
        optionB[i] = tempB;
        optionC[i] = tempC;
        optionD[i] = tempD;
        correctAnswer[i] = tempCorrect;
        // Check if time ran out
        if (timeUp) 
        {
            cout << "\nTime's up for this quiz session!\n";
            break;
        }
        // Pause before next question if not the last question and not time up
        if (i < questionsToAsk - 1 && !timeUp) 
        {
            cout << "\nPress Enter for next question...";
            string temp;
            getline(cin, temp);
        }
    }
}

// Simple timer check function
bool checkTimeExpired(int startTime, int timeLimit) 
{
    return (time(0) - startTime) >= timeLimit;
}

// Display timer (simple version)
void displayTimer(int remainingTime) 
{
    cout << "Time remaining: " << remainingTime << " seconds" << endl;
}

// Function to ask a single question with timer
bool askSingleQuestion(int questionIndex, int questionNum) 
{
    int playerChoice = 0;
    bool timeExpired = false;
    bool lifelineUsed = false;
    int timeForQuestion = question_time;
    // Display question number
    cout << endl << "Question " << questionNum << ":\n";
    cout << "-----------------------------------------\n";
    // Display question
    cout << questionText[questionIndex] << "\n\n";
    // Display options
    cout << "1. " << optionA[questionIndex] << "\n";
    cout << "2. " << optionB[questionIndex] << "\n";
    cout << "3. " << optionC[questionIndex] << "\n";
    cout << "4. " << optionD[questionIndex] << "\n";
    // Display lifelines
    cout << "\nLifelines: ";
    if (lifelinesLeft > 0) 
    {
        cout << "50/50 (" << (lifelinesLeft - (extraTimeUsed ? 1 : 0)) << " left)";
        if (!extraTimeUsed)
            cout << ", Extra Time (+10s)";
    }
    else
        cout << "None available";
    cout << endl;
    // Timer implementation
    time_t startTime = time(0);
    bool answered = false;
    while (!answered && !timeExpired) 
    {
        // Calculate remaining time
        int elapsedTime = difftime(time(0), startTime);
        int remainingTime = timeForQuestion - elapsedTime;
        if (remainingTime <= 0) 
        {
            timeExpired = true;
            break;
        }
        // Display timer
        cout << "Time: " << remainingTime << "s | Enter choice (1-4) or L for lifeline: ";
        // Check for input
        if (cin.peek() != EOF) //Checks if there is any input waiting to be read without actually consuming it
        {
            string input;
            getline(cin, input);
            // Check for lifeline
            if ((input == "L" || input == "l") && lifelinesLeft > 0) 
            {
                cout << "Choose lifeline:\n";
                cout << "1. 50/50 (Remove two wrong options)\n";
                if (!extraTimeUsed) 
                {
                    cout << "2. Extra Time (+10 seconds)\n";
                }
                cout << "Enter choice: ";
                string lifelineChoice;
                getline(cin, lifelineChoice);
                if (lifelineChoice == "1") 
                {
                    applyLifeline(questionIndex);
                    lifelinesLeft--;
                    lifelineUsed = true;
                    // Redisplay question with lifeline applied
                    cout << "\nLifeline used! Two wrong options removed.\n";
                    cout << questionText[questionIndex] << "\n\n";
                    cout << "1. " << optionA[questionIndex] << "\n";
                    cout << "2. " << optionB[questionIndex] << "\n";
                    cout << "3. " << optionC[questionIndex] << "\n";
                    cout << "4. " << optionD[questionIndex] << "\n";
                    cout << "\n";
                }
                else if (lifelineChoice == "2" && !extraTimeUsed) 
                {
                    timeForQuestion += 10;
                    lifelinesLeft--;
                    extraTimeUsed = true;
                    startTime = time(0); // Reset timer with new time
                    cout << "\nExtra time lifeline used! +10 seconds added.\n";
                    cout << "New time: " << timeForQuestion << " seconds\n";
                }
                else 
                    cout << "Invalid lifeline choice or lifeline not available.\n";
                continue;
            }
            // Convert input to integer
            playerChoice = atoi(input.c_str());
            // Validate choice
            if (playerChoice >= 1 && playerChoice <= 4)
                answered = true;
            else 
                cout << "Invalid choice! Please enter 1-4 or L.\n";
        }
        // Small delay to prevent CPU overuse
        // Simple delay for 100ms
        clock_t delayStart = clock();
        while (clock() < delayStart + 100);
    }

    if (timeExpired) 
    {
        cout <<endl <<  "Time's up! ";
        // Show correct answer
        string correctOptionText;
        if (correctAnswer[questionIndex] == 1) 
            correctOptionText = optionA[questionIndex];
        else if (correctAnswer[questionIndex] == 2) 
            correctOptionText = optionB[questionIndex];
        else if (correctAnswer[questionIndex] == 3) 
            correctOptionText = optionC[questionIndex];
        else 
            correctOptionText = optionD[questionIndex];
        cout << "Correct answer was: " << correctOptionText << "\n";
        // DEDUCT POINTS FOR TIMEOUT (NEW FEATURE)
        ++wrongAnswers;
        correctStreak = 0;
        // Deduct points based on difficulty for timeout
        int penalty = 0;
        if (currentDifficulty == 1) 
            penalty = 2;  // -2 points for Easy
        else if (currentDifficulty == 2) 
            penalty = 3;  // -3 points for Medium
        else 
            penalty = 5;  // -5 points for Hard
        playerScore -= penalty;
        cout << "Timeout penalty: -" << penalty << " points\n";
        // Store wrong question for review
        if (wrongQuestionCount < 10) 
        {
            wrongQuestionText[wrongQuestionCount] = questionText[questionIndex];
            wrongOptionA[wrongQuestionCount] = optionA[questionIndex];
            wrongOptionB[wrongQuestionCount] = optionB[questionIndex];
            wrongOptionC[wrongQuestionCount] = optionC[questionIndex];
            wrongOptionD[wrongQuestionCount] = optionD[questionIndex];
            wrongCorrectAnswer[wrongQuestionCount] = correctAnswer[questionIndex];
            wrongQuestionCount++;
        }
        timeUp = true;
        return false;
    }
    // Check if answer is correct (only reached if answered before timeout)
    if (playerChoice == correctAnswer[questionIndex]) 
    {
        cout << "\nCorrect! ";
        correctAnswers++;
        correctStreak++;
        // Apply streak bonus
        if (correctStreak >= 5) 
        {
            playerScore += 15;
            cout << "5-question streak! +15 points! ";
            correctStreak = 0; // Reset streak
        }
        else if (correctStreak >= 3) 
        {
            playerScore += 5;
            cout << "3-question streak! +5 points! ";
        }
        // Add points based on difficulty
        int points = 0;
        if (currentDifficulty == 1) 
            points = 2;
        else if (currentDifficulty == 2) 
            points = 3;
        else 
            points = 5;
        playerScore += points;
        cout << "+" << points << " points\n";
        return true;
    }
    else 
    {
        cout << endl << "Wrong! ";
        // Show correct answer
        string correctOptionText;
        if (correctAnswer[questionIndex] == 1) correctOptionText = optionA[questionIndex];
        else if (correctAnswer[questionIndex] == 2) correctOptionText = optionB[questionIndex];
        else if (correctAnswer[questionIndex] == 3) correctOptionText = optionC[questionIndex];
        else correctOptionText = optionD[questionIndex];
        cout << "Correct answer was: " << correctOptionText << "\n";
        ++wrongAnswers;
        correctStreak = 0; // Reset streak on wrong answer
        // Deduct points based on difficulty for wrong answer
        int penalty = 0;
        if (currentDifficulty == 1) 
            penalty = 2;
        else if (currentDifficulty == 2) 
            penalty = 3;
        else 
            penalty = 5;
        playerScore -= penalty;
        cout << "-" << penalty << " points\n";

        return false;
    }
}

// Function to apply lifeline
void applyLifeline(int questionIndex) 
{
    int wrongCount = 0;
    // Hide two wrong options
    for (int optionNum = 1; optionNum <= 4 && wrongCount < 2; optionNum++) 
    {
        if (optionNum != correctAnswer[questionIndex]) 
        {
            if (optionNum == 1) 
                optionA[questionIndex] = "(Hidden)";
            else if (optionNum == 2) 
                optionB[questionIndex] = "(Hidden)";
            else if (optionNum == 3) 
                optionC[questionIndex] = "(Hidden)";
            else 
                optionD[questionIndex] = "(Hidden)";
            ++wrongCount;
        }
    }
}

// Function to display final results
void displayFinalResults() 
{
    cout << endl << "=============================================\n";
    cout << "            QUIZ COMPLETED!\n";
    cout << "=============================================\n";
    cout << "Player Name: " << playerName << "\n";
    cout << "Category: " << currentCategory << "\n";
    cout << "Difficulty: ";
    if (currentDifficulty == 1) 
        cout << "Easy";
    else if (currentDifficulty == 2) 
        cout << "Medium";
    else 
        cout << "Hard";
    cout << endl << "Time per question: " << question_time << " seconds\n";
    cout << "---------------------------------------------\n";
    cout << "Correct Answers: " << correctAnswers << "/10\n";
    cout << "Wrong Answers: " << wrongAnswers << "/10\n";
    if (timeUp)
        cout << "Quiz ended early due to timeout!\n";
    cout << "FINAL SCORE: " << playerScore << "\n";
    // Display performance message
    cout << "\nPerformance: ";
    if (correctAnswers >= 8) 
        cout << "Excellent!\n";
    else if (correctAnswers >= 6) 
        cout << "Good Job!\n";
    else if (correctAnswers >= 4) 
        cout << "Not Bad!\n";
    else 
        cout << "Keep Practicing!\n";
    cout << "=============================================\n";
}

// Function to save player score (SIMPLIFIED)
void savePlayerScore() 
{
    // Save to high scores file
    ofstream scoreFile("high_scores.txt", ofstream::app);
    if (scoreFile.is_open()) 
    {
        scoreFile << playerName << endl;
        scoreFile << playerScore << endl;
        scoreFile << currentCategory << endl;
        scoreFile << "---" << endl; // Separator
        scoreFile.close();
        cout << "Score saved to high_scores.txt\n";
    }
    else 
        cout << "Error saving score!\n";
    // Save to quiz logs
    ofstream logFile("quiz_logs.txt", ofstream::app);
    if (logFile.is_open()) 
    {
        logFile << "Player: " << playerName << endl;
        logFile << "Score: " << playerScore << endl;
        logFile << "Category: " << currentCategory << endl;
        logFile << "Correct: " << correctAnswers << "/10" << endl;
        logFile << "===============" << endl << endl;
        logFile.close();
    }
}

// Function to show high scores
void showHighScores() 
{
    ifstream scoreFile("high_scores.txt");
    cout << "=============================================\n";
    cout << "            HIGH SCORES\n";
    cout << "=============================================\n";

    if (!scoreFile.is_open()) 
    {
        cout << "No high scores yet. Be the first to play!\n";
        return;
    }
    string name;
    string score;
    string category;
    string separator;
    int count = 0;

    // Read and display scores
    while (count < 5 && getline(scoreFile, name)) 
    {
        if (name.empty()) continue;
        if (!getline(scoreFile, score)) break;
        if (!getline(scoreFile, category)) break;
        if (!getline(scoreFile, separator)) break;
        cout << (count + 1) << ". " << name << " - " << score<< " points (" << category << ")\n";
        count++;
    }
    if (count == 0) 
    {
        cout << "No high scores yet. Play a game to add one!\n";
    }
    scoreFile.close();
}

// Function to review wrong answers
void reviewWrongAnswers() 
{
    if (wrongQuestionCount == 0) 
    {
        cout << endl << "No wrong answers to review!\n";
        return;
    }
    cout << endl << "=============================================\n";
    cout << "        REVIEW OF WRONG ANSWERS\n";
    cout << "=============================================\n";
    cout << "You answered " << wrongQuestionCount << " questions incorrectly.\n\n";
    for (int i = 0; i < wrongQuestionCount; i++) 
    {
        cout << "Question " << (i + 1) << ":\n";
        cout << wrongQuestionText[i] << endl;
        cout << "1. " << wrongOptionA[i];
        if (wrongCorrectAnswer[i] == 1) 
            cout << " <-- CORRECT ANSWER";
        cout << endl;
        cout << "2. " << wrongOptionB[i];
        if (wrongCorrectAnswer[i] == 2) 
            cout << " <-- CORRECT ANSWER";
        cout << endl;
        cout << "3. " << wrongOptionC[i];
        if (wrongCorrectAnswer[i] == 3) 
            cout << " <-- CORRECT ANSWER";
        cout << endl;
        cout << "4. " << wrongOptionD[i];
        if (wrongCorrectAnswer[i] == 4) 
            cout << " <-- CORRECT ANSWER";
        cout << endl;
        cout << endl << "-----------------------------------------\n\n";
    }
}

// Function to replay the last quiz
void replayQuiz() 
{
    if (totalQuestionsLoaded == 0) 
    {
        cout << "\nNo previous quiz found. Please start a new quiz first.\n";
        return;
    }
    char choice;
    cout << "\nDo you want to replay the last quiz in " << currentCategory << " category? (y/n): ";
    cin >> choice;
    clearInputBuffer();
    if (choice == 'y' || choice == 'Y') 
    {
        // Reset scores for replay
        playerScore = 0;
        correctAnswers = 0;
        wrongAnswers = 0;
        wrongQuestionCount = 0;
        correctStreak = 0;
        lifelinesLeft = 2;
        extraTimeUsed = false;
        timeUp = false;
        // Shuffle questions again for replay
        shuffleQuestions();
        cout << "\n=============================================\n";
        cout << "   QUIZ REPLAY STARTED\n";
        cout << "   Time per question: " << question_time << " seconds\n";
        cout << "=============================================\n\n";
        // Ask questions again
        askQuestions();
        // Show final results
        displayFinalResults();
        // Save the score
        savePlayerScore();
    }
}