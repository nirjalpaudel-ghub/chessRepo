#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>
#include <Windows.h>

enum class PieceType {
    None, King, Queen, Rook, Bishop, Knight, Pawn
};

enum class Color {
    White, Black
};

enum class GameState {
    Playing, Check, Checkmate, Stalemate
};

enum class MenuState {
    MainMenu, InGame
};

struct Move {
    sf::Vector2i from;
    sf::Vector2i to;
    PieceType capturedPiece = PieceType::None;
    bool isCastling = false;
    bool isEnPassant = false;
    bool isPromotion = false;
    PieceType promotionPiece = PieceType::Queen;
};

struct Piece {
    PieceType type = PieceType::None;
    Color color = Color::White;
    sf::Sprite sprite;
    bool hasMoved = false;

    void setTexture(sf::Texture& texture, int spriteX, int spriteY, int pieceWidth, int pieceHeight) {
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(spriteX, spriteY, pieceWidth, pieceHeight));

        float scaleX = 80.0f / pieceWidth;
        float scaleY = 80.0f / pieceHeight;
        sprite.setScale(scaleX, scaleY);

        sprite.setOrigin(pieceWidth / 2.0f, pieceHeight / 2.0f);
    }
};

class ChessGame {
private:
    sf::RenderWindow window;
    std::vector<std::vector<Piece>> board;
    sf::Texture piecesTexture;
    sf::Font font;
    bool isPieceSelected = false;
    sf::Vector2i selectedPosition;
    Color currentTurn = Color::White;
    GameState gameState = GameState::Playing;
    MenuState menuState = MenuState::MainMenu;
    
    //
    sf::Vector2i lastMoveFrom = { -1, -1 };
    sf::Vector2i lastMoveTo = { -1, -1 };
    PieceType lastMovePieceType = PieceType::None;
    //
    // sound 
    sf::SoundBuffer moveBuffer;
    sf::SoundBuffer captureBuffer;

    sf::Sound moveSound;
    sf::Sound captureSound;

    // Special move tracking
    sf::Vector2i enPassantTarget = { -1, -1 };
    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteRookKingsideMoved = false;
    bool whiteRookQueensideMoved = false;
    bool blackRookKingsideMoved = false;
    bool blackRookQueensideMoved = false;

    std::vector<Move> moveHistory;
    //rotate board
    bool rotateBoard = true;
    // Menu variables
    int selectedMenuItem = 0;
    std::vector<std::string> menuItems = { "Single Player", "Multiplayer", "Quit" };

public:
    ChessGame();
    void run();

private:
    void initializeBoard();
    void loadSounds();
    void loadTextures();
    void drawBoard();
    void drawPieces();
    void drawSelection();
    void drawMenu();
    void handleMouseClick(sf::Vector2i mousePos);
    void handleMenuClick(sf::Vector2i mousePos);
    void handleKeyPress(sf::Keyboard::Key key);
    void switchTurn();

    // Movement validation
    bool isValidMove(sf::Vector2i from, sf::Vector2i to) const;
    bool isValidPawnMove(sf::Vector2i from, sf::Vector2i to, const Piece& pawn, const Piece& target) const;
    bool isValidRookMove(sf::Vector2i from, sf::Vector2i to) const;
    bool isValidBishopMove(sf::Vector2i from, sf::Vector2i to) const;
    bool isValidQueenMove(sf::Vector2i from, sf::Vector2i to) const;
    bool isValidKnightMove(sf::Vector2i from, sf::Vector2i to) const;
    bool isValidKingMove(sf::Vector2i from, sf::Vector2i to) const;

    // Path checking
    bool isPathClear(sf::Vector2i from, sf::Vector2i to) const;

    // Special moves
    bool canCastle(Color color, bool kingside) const;
    bool isEnPassantValid(sf::Vector2i from, sf::Vector2i to) const;
    void performCastling(sf::Vector2i kingFrom, sf::Vector2i kingTo);
    void performEnPassant(sf::Vector2i from, sf::Vector2i to);

    // Check detection
    bool isInCheck(Color color) const;
    bool isSquareAttacked(sf::Vector2i square, Color byColor) const;
    sf::Vector2i findKing(Color color) const;

    // Game state
    std::vector<sf::Vector2i> getValidMoves(sf::Vector2i position) const;
    std::vector<sf::Vector2i> getAllValidMoves(Color color) const;
    void updateGameState();
    bool wouldBeInCheck(sf::Vector2i from, sf::Vector2i to, Color color) const;

    // Move execution
    void movePiece(sf::Vector2i from, sf::Vector2i to);
    void undoMove(const Move& move);

    // Utility
    bool isInBounds(sf::Vector2i pos) const;
    Color oppositeColor(Color color) const;
    void resetGame();
};