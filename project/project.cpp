#include <iostream>
#include "ConsoleLib.h"
using namespace std;

// Константы
const int Suit = 4;
const int Value = 13;
const int MaxSize = 52;

// Перечисления
enum Suits { Diamonds = 1, Hearts = 2, Clubs = 3, Spades = 4 };
enum Values { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jecks, Queens, Kings, Aces };

// Структура карты
struct Card {
    int suit;
    int value;
};

// Прототипы функций
void printMenu(int selectedOption);
void GenerateDeck(Card Deck[], int DeckSize);
void ShufflesDeck(Card Deck[], int DeckSize);
void ShowFlop(const Card Flop[], int FlopSize);
void ShowTurn(const Card Turn[], int TurnSize);
void ShowRiver(const Card River[], int RiverSize);
int GiveHand(Card Deck[], int NumCard, Card Hand[], int HandSize);
int GiveBot(Card Deck[], int NumCard, Card Hand[], int HandSize);
void DealFlop(Card Deck[], int DeckSize, Card Flop[], int& FlopSize);
void DealTurn(Card Deck[], int DeckSize, Card Turn[], int& TurnSize);
void DealRiver(Card Deck[], int DeckSize, Card River[], int& RiverSize);
bool HasPair(const Card hand[], int handSize, const Card table[], int tableSize);
bool HasTwoPairs(const Card hand[], int handSize, const Card table[], int tableSize);
bool HasSet(const Card hand[], int handSize, const Card table[], int tableSize);
bool HasStreet(const Card hand[], int handSize, const Card table[], int tableSize);
bool HasFlush(const Card hand[], int handSize, const Card table[], int tableSize);
bool HasFullHouse(const Card hand[], int handSize, const Card table[], int tableSize);
bool HasFourOfAKind(const Card hand[], int handSize, const Card table[], int tableSize);
bool HasStraightFlush(const Card hand[], int handSize, const Card table[], int tableSize);
void Bet(int& playerChips, int& botChips, int& pot, const char* playerName, const char* botName);
void ResetGame(Card Deck[], int DeckSize, int& playerChips, int& botChips, Card PlayerHand[], int& PlayerHandSize, Card BotHand[], int& BotHandSize);
void PrintPlayerHand(const Card PlayerHand[], int PlayerHandSize);
void PrintBotHand(const Card BotHand[], int BotHandSize);
void DetermineWinner(const Card PlayerHand[], int PlayerHandSize, const Card BotHand[], int BotHandSize,
    const Card River[], int RiverSize, const Card Flop[], int FlopSize, const Card Turn[], int TurnSize, int& playerChips, int& botChips, int& pot);
string GetCardString(const Card& card);
void PrintBanks(int playerChips, int botChips, int pot);
int EvaluateHandRank(const Card hand[], int handSize, const Card table[], int tableSize);

int main() {
    system("title Poker");
    StartRandomize();
    SetConsoleWindowColumnsAndRows(120, 30);
    CenterScreen;
    ShowCursor(false);
    SetColor(BLACK, GREEN);
    SetBkColor(hdc, RGB(0, 255, 0));
    SetTextColor(hdc, RGB(0, 0, 0));

    int selectedOption = 1;
    int MenuOption = 1;

    Card Deck[MaxSize];
    GenerateDeck(Deck, MaxSize);
    ShufflesDeck(Deck, MaxSize);

    Card PlayerHand[2];
    Card BotHand[2];
    Card Flop[3];
    Card Turn[1];
    Card River[1];

    int PlayerHandSize = 0;
    int BotHandSize = 0;
    int FlopSize = 0;
    int TurnSize = 0;
    int RiverSize = 0;

    int playerChips = 1000;
    int botChips = 1000;
    int pot = 0;

    PlayerHandSize = GiveHand(Deck, 2, PlayerHand, PlayerHandSize);
    BotHandSize = GiveBot(Deck, 2, BotHand, BotHandSize);


    while (playerChips > 0 && botChips > 0) {
        // показать главное меню
        system("cls");
        GotoXY(40, 10);
        cout << "==== Welcome to Poker Game ====\n\n";
        GotoXY(50, 12);
        cout << (MenuOption == 1 ? ">" : " ") << "1. Play\n";
        GotoXY(50, 13);
        cout << (MenuOption == 2 ? ">" : " ") << "2. Exit\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72 && MenuOption > 1) { // UP arrow
                MenuOption--;
            }
            else if (key == 80 && MenuOption < 2) { // DOWN arrow
                MenuOption++;
            }
        }
        else if (key == 13) { // ENTER key
            if (MenuOption == 1) {
                // играть
                while (true) {
                    // показать игровое меню
                    system("cls");
                    GotoXY(80, 20);
                    cout << (selectedOption == 1 ? ">" : " ") << "1. FOLD\n";
                    GotoXY(80, 21);
                    cout << (selectedOption == 2 ? ">" : " ") << "2. BET\n";

                    PrintPlayerHand(PlayerHand, PlayerHandSize);
                    PrintBanks(playerChips, botChips, pot);

                    key = _getch();
                    if (key == 224) {
                        key = _getch();
                        if (key == 72 && selectedOption > 1) { // UP arrow
                            selectedOption--;
                        }
                        else if (key == 80 && selectedOption < 2) { // DOWN arrow
                            selectedOption++;
                        }
                    }
                    else if (key == 13) { // ENTER key
                        if (selectedOption == 1) {
                            // FOLD
                            ResetGame(Deck, MaxSize, playerChips, botChips, PlayerHand, PlayerHandSize, BotHand, BotHandSize);
                            // Печать руки игрока после перетасовки  
                            PrintPlayerHand(PlayerHand, PlayerHandSize);
                        }
                        else if (selectedOption == 2) {
                            // BET
                            Bet(playerChips, botChips, pot, "Player", "Bot");
                        
                            if (FlopSize < 3) {
                                DealFlop(Deck, MaxSize, Flop, FlopSize);
                                ShowFlop(Flop, FlopSize);
                            }
                            else if (TurnSize < 1) {
                                DealTurn(Deck, MaxSize, Turn, TurnSize);
                                ShowTurn(Turn, TurnSize);
                            }
                            else if (RiverSize < 1) {
                                DealRiver(Deck, MaxSize, River, RiverSize);
                                ShowRiver(River, RiverSize);
                            }
                            else {
                                // Печать карт после всех ставок и раздачи
                                PrintBotHand(BotHand, BotHandSize);
                        
                                // Оценка комбинации
                                DetermineWinner(PlayerHand, 2, BotHand, 2, River, RiverSize, Flop, FlopSize, Turn, TurnSize, playerChips, botChips, pot);
                        
                                // Печать руки игрока после перетасовки  
                                PrintPlayerHand(PlayerHand, PlayerHandSize);
                                // Сбросить значения Flop, Turn и River
                                FlopSize = TurnSize = RiverSize = 0;
                        
                                // Обновление банков и сброс раунда
                                ResetGame(Deck, MaxSize, playerChips, botChips, PlayerHand, PlayerHandSize, BotHand, BotHandSize);
                            }
                        }
                    }
                    if (botChips <= 0) {
                        // Бот проиграл
                        MessageBoxA(0, "Congratulations! You won!", "Game Over", MB_OK);
                        exit(0);
                        
                    }
                    else if (playerChips <= 0) {
                        // Вы проиграли
                        MessageBoxA(0, "Sorry, you lost. Better luck next time!", "Game Over", MB_OK);
                        exit(0);
                        
                    }
                }
            }
            else if (MenuOption == 2) {
                // Exit
                exit(0); // Закрыть программу
            }
        }
    }

    cin.get();

    SetBkColor(hdc, RGB(0, 255, 0));
    SetTextColor(hdc, RGB(255, 255, 255));

    Sleep(INFINITE);
}

void printMenu(int selectedOption) {
    system("cls");
    GotoXY(80, 20);
    cout << (selectedOption == 1 ? ">" : " ") << "1. FOLD\n";
    GotoXY(80, 21);
    cout << (selectedOption == 2 ? ">" : " ") << "2. BET\n";
}

void GenerateDeck(Card Deck[], int DeckSize) {
    int suits[] = { Diamonds, Hearts, Clubs, Spades };
    int values[] = { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jecks, Queens, Kings, Aces };

    int CardIndex = 0;
    for (int SuitIndex = 0; SuitIndex < Suit; SuitIndex++) {
        for (int ValueIndex = 0; ValueIndex < Value; ValueIndex++) {
            Deck[CardIndex].suit = suits[SuitIndex];
            Deck[CardIndex].value = values[ValueIndex];
            CardIndex++;
        }
    }
}

void ShufflesDeck(Card Deck[], int DeckSize) {
    srand(time(NULL));
    for (int i = 0; i < DeckSize; i++) {
        int j = rand() % DeckSize;
        Card temp = Deck[i];
        Deck[i] = Deck[j];
        Deck[j] = temp;
    }
}

void ShowFlop(const Card Flop[], int FlopSize) {
    GotoXY(30, 5);
    cout << "\nFlop:\n";
    for (int i = 0; i < FlopSize; i++) {
        GotoXY(80, 6 + i);
        cout << GetCardString(Flop[i]) << "\n";
    }
}

void ShowTurn(const Card Turn[], int TurnSize) {
    GotoXY(30, 11);
    cout << "\nTurn:\n";
    for (int i = 0; i < TurnSize; i++) {
        GotoXY(80, 12 + i);
        cout << GetCardString(Turn[i]) << "\n";
    }
}

void ShowRiver(const Card River[], int RiverSize) {
    GotoXY(30, 13);
    cout << "\nRiver:\n";
    for (int i = 0; i < RiverSize; i++) {
        GotoXY(80, 14 + i);
        cout << GetCardString(River[i]) << "\n";
    }
}

int GiveHand(Card Deck[], int NumCard, Card Hand[], int HandSize) {
    for (int i = 0; i < NumCard; i++) {
        Hand[HandSize++] = Deck[i];
    }
    return HandSize;
}

int GiveBot(Card Deck[], int NumCard, Card Hand[], int HandSize) {
    for (int i = 0; i < NumCard; i++) {
        Hand[HandSize++] = Deck[HandSize + NumCard];
    }
    return HandSize;
}

void DealFlop(Card Deck[], int DeckSize, Card Flop[], int& FlopSize) {
    for (int i = 0; i < 3; i++) {
        Flop[FlopSize++] = Deck[DeckSize - 1];
        DeckSize--;
    }
    ShowFlop(Flop, FlopSize);
}

void DealTurn(Card Deck[], int DeckSize, Card Turn[], int& TurnSize) {
    Turn[TurnSize++] = Deck[DeckSize - 4];
    DeckSize--;
    ShowTurn(Turn, TurnSize);
}

void DealRiver(Card Deck[], int DeckSize, Card River[], int& RiverSize) {
    River[RiverSize++] = Deck[DeckSize - 5];
    DeckSize--;
    ShowRiver(River, RiverSize);
}

bool HasPair(const Card hand[], int handSize, const Card table[], int tableSize) {
    for (int i = 0; i < handSize; i++) {
        for (int j = 0; j < tableSize; j++) {
            if (hand[i].value == table[j].value) {
                return true;
            }
        }
    }
    return false;
}

bool HasTwoPairs(const Card hand[], int handSize, const Card table[], int tableSize) {
    int valueCounts[15] = { 0 };

    for (int i = 0; i < handSize; i++) {
        valueCounts[hand[i].value]++;
    }
    for (int i = 0; i < tableSize; i++) {
        valueCounts[table[i].value]++;
    }

    int pairCount = 0;
    for (int i = 2; i <= 14; i++) {
        if (valueCounts[i] == 2) {
            pairCount++;
        }
    }

    return pairCount >= 2;
}

bool HasSet(const Card hand[], int handSize, const Card table[], int tableSize) {
    for (int i = 0; i < handSize; i++) {
        int count = 1;
        for (int j = 0; j < tableSize; j++) {
            if (hand[i].value == table[j].value) {
                count++;
                if (count == 3) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool HasStreet(const Card hand[], int handSize, const Card table[], int tableSize) {
    int allCardsSize = handSize + tableSize;
    Card* allCards = new Card[allCardsSize];

    for (int i = 0; i < handSize; i++) {
        allCards[i] = hand[i];
    }
    for (int i = 0; i < tableSize; i++) {
        allCards[handSize + i] = table[i];
    }

    // Sort all cards by value
    for (int i = 0; i < allCardsSize - 1; i++) {
        for (int j = 0; j < allCardsSize - i - 1; j++) {
            if (allCards[j].value > allCards[j + 1].value) {
                // swap
                Card temp = allCards[j];
                allCards[j] = allCards[j + 1];
                allCards[j + 1] = temp;
            }
        }
    }

    int consecutiveCount = 1;
    for (int i = 0; i < allCardsSize - 1; i++) {
        if (allCards[i + 1].value - allCards[i].value == 1) {
            consecutiveCount++;
            if (consecutiveCount == 5) {
                return true;
            }
        }
        else if (allCards[i + 1].value != allCards[i].value) {
            consecutiveCount = 1;
        }
    }

    delete[] allCards;
    return false;
}

bool HasFlush(const Card hand[], int handSize, const Card table[], int tableSize) {
    int suitsCount[5] = { 0 };

    for (int i = 0; i < handSize; i++) {
        suitsCount[hand[i].suit]++;
    }
    for (int i = 0; i < tableSize; i++) {
        suitsCount[table[i].suit]++;
    }

    for (int i = 1; i <= 4; i++) {
        if (suitsCount[i] >= 5) {
            return true;
        }
    }

    return false;
}

bool HasFullHouse(const Card hand[], int handSize, const Card table[], int tableSize) {
    return HasSet(hand, handSize, table, tableSize) && HasPair(hand, handSize, table, tableSize);
}

bool HasFourOfAKind(const Card hand[], int handSize, const Card table[], int tableSize) {
    int valueCounts[15] = { 0 };

    for (int i = 0; i < handSize; i++) {
        valueCounts[hand[i].value]++;
    }
    for (int i = 0; i < tableSize; i++) {
        valueCounts[table[i].value]++;
    }

    for (int i = 2; i <= 14; i++) {
        if (valueCounts[i] == 4) {
            return true;
        }
    }

    return false;
}

bool HasStraightFlush(const Card hand[], int handSize, const Card table[], int tableSize) {
    return HasStreet(hand, handSize, table, tableSize) && HasFlush(hand, handSize, table, tableSize);
}

void Bet(int& playerChips, int& botChips, int& pot, const char* playerName, const char* botName) {
    GotoXY(20, 22);
    int playerBet;
    cout << "\r" << playerName << ", enter your bet: " << "                   ";  // Очистка предыдущего значения
    cout << "\r" << playerName << ", enter your bet: ";

    while (true) {
        cin >> playerBet;

        // Update chip counts and pot
        playerChips -= playerBet;
        botChips -= playerBet;
        pot += 2 * playerBet;

        if (playerBet > 0) {
            break;
        }
        else {
            cout << "\rInvalid bet. Please enter a positive value: ";
        }
    }
}

void ResetGame(Card Deck[], int DeckSize, int& playerChips, int& botChips, Card PlayerHand[], int& PlayerHandSize, Card BotHand[], int& BotHandSize) {
    GenerateDeck(Deck, MaxSize);
    ShufflesDeck(Deck, MaxSize);
    DeckSize = MaxSize;

    // Сброс флопа, тёрна и ривера
    Card Flop[3];
    int FlopSize = 0;
    Card Turn[1];
    int TurnSize = 0;
    Card River[1];
    int RiverSize = 0;

    // Сброс ставок
    int pot = 0;

    // Раздача карт игроку и боту
    PlayerHandSize = GiveHand(Deck, 2, PlayerHand, 0);
    BotHandSize = GiveBot(Deck, 2, BotHand, 0);

    // Очистка экрана
    system("cls");

    // Печать руки игрока после сброса
    PrintPlayerHand(PlayerHand, PlayerHandSize);

    // Обновление банков и ставок
    PrintBanks(playerChips, botChips, pot);
}

void PrintPlayerHand(const Card PlayerHand[], int PlayerHandSize) {
    GotoXY(30, 1);
    cout << "\nPlayer Hand:\n";
    for (int i = 0; i < PlayerHandSize; i++) {
        GotoXY(80, 2 + i);
        cout << GetCardString(PlayerHand[i]) << "\n";
    }
}

void PrintBotHand(const Card BotHand[], int BotHandSize) {
    GotoXY(30, 15);  // Изменено с 12 на 14
    cout << "\nBot Hand:\n";
    for (int i = 0; i < BotHandSize; i++) {
        GotoXY(80, 16 + i);
        cout << GetCardString(BotHand[i]) << "\n";
    }
}

// Функция определения победителя
void DetermineWinner(const Card PlayerHand[], int PlayerHandSize, const Card BotHand[], int BotHandSize,
    const Card River[], int RiverSize, const Card Flop[], int FlopSize, const Card Turn[], int TurnSize, int& playerChips, int& botChips, int& pot) {

    // Оценка комбинации игрока
    int playerRank = EvaluateHandRank(PlayerHand, PlayerHandSize, River, RiverSize);

    // Оценка комбинации бота
    int botRank = EvaluateHandRank(BotHand, BotHandSize, River, RiverSize);

    GotoXY(80, 28);
    if (playerRank > botRank) {
        cout << "Player wins!\n";
        playerChips += pot;  // Передача выигрыша игроку
    }
    else if (playerRank < botRank) {
        cout << "Bot wins!\n";
        botChips += pot;  // Передача выигрыша боту
    }
    else {
        cout << "It's a tie!\n";
        // В случае ничьи, делим банк между игроком и ботом
        playerChips += pot / 2;
        botChips += pot / 2;
    }
    pot = 0;  // Обнуление pot для нового раунда

    // Обновление банков
    PrintBanks(playerChips, botChips, pot);
}

// Функция для получения строки с названием карты
string GetCardString(const Card& card) {
    string suitStr;
    switch (card.suit) {
    case Diamonds:
        suitStr = " Diamonds";
        break;
    case Hearts:
        suitStr = " Hearts";
        break;
    case Clubs:
        suitStr = " Clubs";
        break;
    case Spades:
        suitStr = " Spades";
        break;
    }

    string valueStr;
    switch (card.value) {
    case Two:
        valueStr = "2";
        break;
    case Three:
        valueStr = "3";
        break;
    case Four:
        valueStr = "4";
        break;
    case Five:
        valueStr = "5";
        break;
    case Six:
        valueStr = "6";
        break;
    case Seven:
        valueStr = "7";
        break;
    case Eight:
        valueStr = "8";
        break;
    case Nine:
        valueStr = "9";
        break;
    case Ten:
        valueStr = "10";
        break;
    case Jecks:
        valueStr = "J";
        break;
    case Queens:
        valueStr = "Q";
        break;
    case Kings:
        valueStr = "K";
        break;
    case Aces:
        valueStr = "A";
        break;
    }

    return valueStr + suitStr;
}

// Функция для печати текущих банков
void PrintBanks(int playerChips, int botChips, int pot) {
    GotoXY(80, 26);
    cout << "Player Bank: " << playerChips << " | Bot Bank: " << botChips << "\n";
    GotoXY(80, 27);
    cout << "Pot: " << pot << "\n";
}

int EvaluateHandRank(const Card hand[], int handSize, const Card table[], int tableSize) {
    if (HasStraightFlush(hand, handSize, table, tableSize)) {
        return 8; // Стрит-флеш
    }
    else if (HasFourOfAKind(hand, handSize, table, tableSize)) {
        return 7; // Каре
    }
    else if (HasFullHouse(hand, handSize, table, tableSize)) {
        return 6; // Фулл-хаус
    }
    else if (HasFlush(hand, handSize, table, tableSize)) {
        return 5; // Флеш
    }
    else if (HasStreet(hand, handSize, table, tableSize)) {
        return 4; // Стрит
    }
    else if (HasSet(hand, handSize, table, tableSize)) {
        return 3; // Сет
    }
    else if (HasTwoPairs(hand, handSize, table, tableSize)) {
        return 2; // Две пары
    }
    else if (HasPair(hand, handSize, table, tableSize)) {
        return 1; // Пара
    }
    else {
        return 0; // Нет комбинации
    }
}