# Console-Based Quiz Game

## Project Overview

A fully interactive console-based trivia game developed as the Final Lab Project for CS1002 - Programming Fundamentals. This dynamic quiz system challenges players with multiple-choice questions across five categories, featuring difficulty levels, timers, lifelines, streak bonuses, and persistent high score tracking.

## Team Members
25F-0594 Sana Irfan
25F-0627 Dania Mustafa
25F-0690 Hijab Fatima

## Features

### Core Gameplay
- 5 Categories: Science, Computer, Sports, History, IQ/Logic
- 3 Difficulty Levels: Easy (+2/-2), Medium (+3/-3), Hard (+5/-5)
- 10 Random Questions per quiz session
- Timed Questions (10 seconds per question)

### Gamification Elements
- Streak Bonus System:
  - +5 points after 3 consecutive correct answers
  - +15 points after 5 consecutive correct answers
- Negative Marking: Points deducted for wrong answers based on difficulty
- Timeout Penalty: Points deducted if time runs out

### Lifelines (KBC-Inspired)
- 50/50 – Removes two wrong options
- Extra Time – Adds 10 more seconds

### Data Persistence
- High Scores saved to high_scores.txt
- Quiz Logs saved to quiz_logs.txt with player name, score, category, and performance
- Wrong Answer Review – Review the incorrectly answered questions after quiz

### Additional Features
- Question Shuffling – Random order in each session
- Option Shuffling – Random order of answer choices
- Replay Functionality – Replay the last quiz
- Performance Feedback – Personalized messages based on score

## How to Run

### Prerequisites
- Microsoft Visual Studio (recommended) or any C++ compiler
- C++11 or higher

### Setup Instructions
1. Clone or download the project files
2. Ensure all .txt files (question banks) are in the same directory as the executable:
   - science.txt
   - computer.txt
   - sports.txt
   - history.txt
   - IQ.txt
3. Open the project in Visual Studio
4. Build and run the solution

### File Structure
```
Project Fall25/
├── new.cpp                 # Main source code
├── science.txt            # Science question bank
├── computer.txt           # Computer question bank
├── sports.txt             # Sports question bank
├── history.txt            # History question bank
├── IQ.txt                 # IQ/Logic question bank
├── high_scores.txt        # Generated - stores high scores
├── quiz_logs.txt          # Generated - stores quiz sessions
└── DOCUMENTATION.docx     # Project documentation with outputs
```

## How to Play

1. Launch the game – Main menu displays options
2. Enter your name – For score tracking
3. Choose a category – Science, Computer, Sports, History, or IQ/Logic
4. Select difficulty – Easy, Medium, or Hard
5. Answer 10 questions – Each with a 10-second timer
6. Use lifelines – Type L during a question
7. Review results – See final score and review wrong answers

### Controls
- 1-4 – Select answer choice
- L – Access lifelines menu
- y/n – Confirm choices

## Technical Implementation

### Key Functions
loadQuestions() -> Reads questions from category files
shuffleQuestions() -> Randomizes question order
shuffleOptions() -> Randomizes answer choices
askSingleQuestion() -> Handles timer, lifelines, scoring
applyLifeline() -> Implements 50/50 lifeline
savePlayerScore() -> Writes to high_scores.txt and quiz_logs.txt
reviewWrongAnswers() -> Displays incorrect questions

### Data Structures
- Parallel arrays for question storage (max 100 questions)
- Separate arrays for wrong answer review (max 10 questions)
- File streams for reading question banks and writing logs

## Sample Output Screenshots

Refer to DOCUMENTATION.docx for complete output screenshots including:
- Main menu display
- Category selection
- Difficulty selection
- Question display with timer
- Lifeline usage
- Final results screen
- High scores leaderboard
- Wrong answers review

## Known Limitations
- Maximum 100 questions per category file
- Maximum 10 wrong answers stored for review
- Timer input handling uses non-blocking approach with small delay

## Future Enhancements
- Multiplayer mode implementation
- Auto-save player progress
- Dynamic question addition by players
- Skip question lifeline
- Replace question lifeline
