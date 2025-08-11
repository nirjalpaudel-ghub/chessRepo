#include "ChessGame.h"
#include <iostream>
#include <cmath>
#include <algorithm>


ChessGame::ChessGame() : window(sf::VideoMode(800, 800), "SFML Chess") {
    initializeBoard();
    loadTextures();
    loadSounds();
    // Try to load a font - with fallback options
    if (!font.loadFromFile("arial.ttf")) {
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            if (!font.loadFromFile("/System/Library/Fonts/Arial.ttf")) {
                if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
                    std::cout << "Warning: Could not load any font. Text may not display properly." << std::endl;
                    // Create a simple fallback - we'll use rectangles for menu items
                }
            }
        }
    }

    window.setFramerateLimit(60);
}

void ChessGame::initializeBoard() {
    board.resize(8, std::vector<Piece>(8));
    

    // Initialize all squares as empty
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = { PieceType::None, Color::White };
        }
    }

    // White pieces (bottom rows)
    board[0][0] = { PieceType::Rook, Color::White };
    board[0][1] = { PieceType::Knight, Color::White };
    board[0][2] = { PieceType::Bishop, Color::White };
    board[0][3] = { PieceType::Queen, Color::White };
    board[0][4] = { PieceType::King, Color::White };
    board[0][5] = { PieceType::Bishop, Color::White };
    board[0][6] = { PieceType::Knight, Color::White };
    board[0][7] = { PieceType::Rook, Color::White };

    for (int i = 0; i < 8; i++) {
        board[1][i] = { PieceType::Pawn, Color::White };
    }

    // Black pieces (top rows)
    board[7][0] = { PieceType::Rook, Color::Black };
    board[7][1] = { PieceType::Knight, Color::Black };
    board[7][2] = { PieceType::Bishop, Color::Black };
    board[7][3] = { PieceType::Queen, Color::Black };
    board[7][4] = { PieceType::King, Color::Black };
    board[7][5] = { PieceType::Bishop, Color::Black };
    board[7][6] = { PieceType::Knight, Color::Black };
    board[7][7] = { PieceType::Rook, Color::Black };

    for (int i = 0; i < 8; i++) {
        board[6][i] = { PieceType::Pawn, Color::Black };
    }
}

void ChessGame::loadTextures() {
    if (!piecesTexture.loadFromFile("D:\\chessGame\\images\\chess_pieces.png")) {
        std::cerr << "Error: Failed to load chess_pieces.png\n";
        sf::Image debugImg;
        debugImg.create(200, 200, sf::Color::Magenta);
        piecesTexture.loadFromImage(debugImg);
        std::cout << "Using debug texture instead\n";
        return;
    }

    std::cout << "Successfully loaded chess pieces texture\n";
    sf::Vector2u textureSize = piecesTexture.getSize();
    int pieceWidth = textureSize.x / 6;
    int pieceHeight = textureSize.y / 2;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col].type != PieceType::None) {
                int spriteX = 0, spriteY = 0;

                switch (board[row][col].type) {
                case PieceType::Rook:    spriteX = 0 * pieceWidth; break;
                case PieceType::Knight:  spriteX = 1 * pieceWidth; break;
                case PieceType::Bishop:  spriteX = 2 * pieceWidth; break;
                case PieceType::Queen:   spriteX = 3 * pieceWidth; break;
                case PieceType::King:    spriteX = 4 * pieceWidth; break;
                case PieceType::Pawn:    spriteX = 5 * pieceWidth; break;
                default: break;
                }

                spriteY = (board[row][col].color == Color::Black) ? 0 : pieceHeight;
                board[row][col].setTexture(piecesTexture, spriteX, spriteY, pieceWidth, pieceHeight);

                // Set initial position
                board[row][col].sprite.setPosition(col * 100 + 50, row * 100 + 50);
            }
        }
    }
}

void ChessGame::loadSounds() {
    if (!moveBuffer.loadFromFile("D:\\chessGame\\sounds\\move.mp3")) {
        std::cerr << "Failed to load move.mp3" << std::endl;
    }
    if (!captureBuffer.loadFromFile("D:\\chessGame\\sounds\\capture.mp3")) {
        std::cerr << "Failed to load capture.mp3" << std::endl;
    }

    moveSound.setBuffer(moveBuffer);
    captureSound.setBuffer(captureBuffer);
}

void ChessGame::drawMenu() {
    window.clear(sf::Color(50, 50, 50));

    // Check if font is loaded
    if (font.getInfo().family.empty()) {
        // Fallback: Draw simple rectangles for menu items
        std::cout << "Drawing fallback menu (no font loaded)" << std::endl;

        // Title area
        sf::RectangleShape titleRect(sf::Vector2f(400, 80));
        titleRect.setPosition(200, 150);
        titleRect.setFillColor(sf::Color(100, 100, 100));
        titleRect.setOutlineThickness(2);
        titleRect.setOutlineColor(sf::Color::White);
        window.draw(titleRect);

        // Menu items as rectangles
        for (size_t i = 0; i < menuItems.size(); i++) {
            sf::RectangleShape menuRect(sf::Vector2f(300, 60));
            menuRect.setPosition(250, 300 + i * 80);

            if (i == selectedMenuItem) {
                menuRect.setFillColor(sf::Color(150, 150, 0)); // Yellow-ish for selected
            }
            else {
                menuRect.setFillColor(sf::Color(80, 80, 80));
            }

            menuRect.setOutlineThickness(2);
            menuRect.setOutlineColor(sf::Color::White);
            window.draw(menuRect);
        }

        // Instructions rectangle
        sf::RectangleShape instructRect(sf::Vector2f(600, 40));
        instructRect.setPosition(100, 650);
        instructRect.setFillColor(sf::Color(60, 60, 60));
        instructRect.setOutlineThickness(1);
        instructRect.setOutlineColor(sf::Color(200, 200, 200));
        window.draw(instructRect);

        return;
    }

    // Normal text-based menu (when font is loaded)
    // Title
    sf::Text title;
    title.setFont(font);
    title.setString("SFML Chess");
    title.setCharacterSize(72);
    title.setFillColor(sf::Color::White);
    title.setPosition(400 - title.getGlobalBounds().width / 2, 150);
    window.draw(title);

    // Menu items
    for (size_t i = 0; i < menuItems.size(); i++) {
        sf::Text menuText;
        menuText.setFont(font);
        menuText.setString(menuItems[i]);
        menuText.setCharacterSize(36);

        if (i == selectedMenuItem) {
            menuText.setFillColor(sf::Color::Yellow);
        }
        else {
            menuText.setFillColor(sf::Color::White);
        }

        menuText.setPosition(400 - menuText.getGlobalBounds().width / 2, 300 + i * 80);
        window.draw(menuText);
    }

    // Instructions
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Use UP/DOWN arrows to navigate, ENTER to select, or click with mouse");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color(200, 200, 200));
    instructions.setPosition(400 - instructions.getGlobalBounds().width / 2, 650);
    window.draw(instructions);
}

void ChessGame::handleMenuClick(sf::Vector2i mousePos) {
    // Check if font is loaded for proper text bounds, otherwise use rectangle bounds
    if (font.getInfo().family.empty()) {
        // Fallback: Check rectangle bounds
        for (size_t i = 0; i < menuItems.size(); i++) {
            sf::FloatRect bounds(250, 300 + i * 80, 300, 60);
            if (bounds.contains(mousePos.x, mousePos.y)) {
                selectedMenuItem = i;
                handleKeyPress(sf::Keyboard::Enter);
                return;
            }
        }
        return;
    }

    // Normal text-based click detection
    for (size_t i = 0; i < menuItems.size(); i++) {
        sf::Text menuText;
        menuText.setFont(font);
        menuText.setString(menuItems[i]);
        menuText.setCharacterSize(36);
        menuText.setPosition(400 - menuText.getGlobalBounds().width / 2, 300 + i * 80);

        sf::FloatRect bounds = menuText.getGlobalBounds();
        if (bounds.contains(mousePos.x, mousePos.y)) {
            selectedMenuItem = i;
            handleKeyPress(sf::Keyboard::Enter);
            return;
        }
    }
}

void ChessGame::handleKeyPress(sf::Keyboard::Key key) {
    //toggle board rotation
    if (key == sf::Keyboard::R) {
        rotateBoard = !rotateBoard;
        std::cout << "Rotate Board: " << (rotateBoard ? "ON" : "OFF") << std::endl;
    }

    //

    if (menuState == MenuState::MainMenu) {
        switch (key) {
        case sf::Keyboard::Up:
            selectedMenuItem = (selectedMenuItem - 1 + menuItems.size()) % menuItems.size();
            std::cout << "Selected menu item: " << selectedMenuItem << " (" << menuItems[selectedMenuItem] << ")" << std::endl;
            break;
        case sf::Keyboard::Down:
            selectedMenuItem = (selectedMenuItem + 1) % menuItems.size();
            std::cout << "Selected menu item: " << selectedMenuItem << " (" << menuItems[selectedMenuItem] << ")" << std::endl;
            break;
        case sf::Keyboard::Enter:
            std::cout << "Activating menu item: " << menuItems[selectedMenuItem] << std::endl;
            switch (selectedMenuItem) {
            case 0: // Single Player
                std::cout << "\n=== Single Player Mode ===\n";
                std::cout << "Single Player mode is not available yet.\n";
                std::cout << "This feature will include AI opponent in future updates.\n";
                std::cout << "For now, you can play Multiplayer (local 2-player mode).\n\n";
                break;
            case 1: // Multiplayer
                std::cout << "\n=== Starting Multiplayer Mode ===\n";
                menuState = MenuState::InGame;
                resetGame();
                break;
            case 2: // Quit
                std::cout << "Quitting game...\n";
                window.close();
                break;
            }
            break;
        case sf::Keyboard::Escape:
            window.close();
            break;
        default:
            break;
        }
    }
    else if (menuState == MenuState::InGame) {
        switch (key) {
        case sf::Keyboard::Escape:
            std::cout << "\nReturning to main menu...\n";
            menuState = MenuState::MainMenu;
            selectedMenuItem = 0;
            break;
        default:
            break;
        }
    }
}

void ChessGame::resetGame() {
    // Reset game state
    isPieceSelected = false;
    currentTurn = Color::White;
    gameState = GameState::Playing;
    enPassantTarget = { -1, -1 };
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteRookKingsideMoved = false;
    whiteRookQueensideMoved = false;
    blackRookKingsideMoved = false;
    blackRookQueensideMoved = false;
    moveHistory.clear();

    // Reinitialize board
    initializeBoard();
    loadTextures();

    std::cout << "White to move first." << std::endl;
    std::cout << "Features:" << std::endl;
    std::cout << "- All piece movements" << std::endl;
    std::cout << "- Castling (king and rook must not have moved)" << std::endl;
    std::cout << "- En passant capture (blue square shows target)" << std::endl;
    std::cout << "- Check detection and prevention" << std::endl;
    std::cout << "- Checkmate and stalemate detection" << std::endl;
    std::cout << "- Pawn promotion to Queen" << std::endl;
    std::cout << "Click on a piece to select it, then click on a destination square to move." << std::endl;
    std::cout << "Press ESC to return to main menu." << std::endl << std::endl;
}

bool ChessGame::isValidMove(sf::Vector2i from, sf::Vector2i to) const {
    if (!isInBounds(from) || !isInBounds(to) || from == to) return false;

    const Piece& movingPiece = board[from.y][from.x];
    const Piece& targetPiece = board[to.y][to.x];

    if (movingPiece.type == PieceType::None) return false;
    if (targetPiece.type != PieceType::None && targetPiece.color == movingPiece.color) return false;

    // Check if move would leave king in check
    if (wouldBeInCheck(from, to, movingPiece.color)) return false;

    switch (movingPiece.type) {
    case PieceType::Pawn:   return isValidPawnMove(from, to, movingPiece, targetPiece);
    case PieceType::Rook:   return isValidRookMove(from, to);
    case PieceType::Bishop: return isValidBishopMove(from, to);
    case PieceType::Queen:  return isValidQueenMove(from, to);
    case PieceType::Knight: return isValidKnightMove(from, to);
    case PieceType::King:   return isValidKingMove(from, to);
    default: return false;
    }
}

bool ChessGame::isValidPawnMove(sf::Vector2i from, sf::Vector2i to, const Piece& pawn, const Piece& target) const {
    int direction = (pawn.color == Color::White) ? 1 : -1;
    int startRow = (pawn.color == Color::White) ? 1 : 6;

    // Forward move
    if (from.x == to.x) {
        if (target.type != PieceType::None) return false;

        // Single step
        if (to.y == from.y + direction) return true;

        // Double step from starting position
        if (from.y == startRow && to.y == from.y + 2 * direction) {
            // Check if the square directly in front is also empty
            if (board[from.y + direction][from.x].type == PieceType::None) {
                return true;
            }
        }
    }
    // Diagonal capture
    else if (abs(to.x - from.x) == 1 && to.y == from.y + direction) {
        // Regular capture
        if (target.type != PieceType::None) return true;

        // En passant capture
        if (isEnPassantValid(from, to)) return true;
    }

    return false;
}

bool ChessGame::isValidRookMove(sf::Vector2i from, sf::Vector2i to) const {
    if (from.x != to.x && from.y != to.y) return false;
    return isPathClear(from, to);
}

bool ChessGame::isValidBishopMove(sf::Vector2i from, sf::Vector2i to) const {
    if (abs(to.x - from.x) != abs(to.y - from.y)) return false;
    return isPathClear(from, to);
}

bool ChessGame::isValidQueenMove(sf::Vector2i from, sf::Vector2i to) const {
    return isValidRookMove(from, to) || isValidBishopMove(from, to);
}

bool ChessGame::isValidKnightMove(sf::Vector2i from, sf::Vector2i to) const {
    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

bool ChessGame::isValidKingMove(sf::Vector2i from, sf::Vector2i to) const {
    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);

    // Regular king move
    if (dx <= 1 && dy <= 1) return true;

    // Castling
    if (dy == 0 && dx == 2) {
        Color color = board[from.y][from.x].color;
        bool kingside = (to.x > from.x);
        return canCastle(color, kingside);
    }

    return false;
}

bool ChessGame::isPathClear(sf::Vector2i from, sf::Vector2i to) const {
    int dx = (to.x > from.x) ? 1 : (to.x < from.x) ? -1 : 0;
    int dy = (to.y > from.y) ? 1 : (to.y < from.y) ? -1 : 0;

    sf::Vector2i current = from;
    current.x += dx;
    current.y += dy;

    while (current != to) {
        if (board[current.y][current.x].type != PieceType::None) return false;
        current.x += dx;
        current.y += dy;
    }

    return true;
}

bool ChessGame::canCastle(Color color, bool kingside) const {
    int row = (color == Color::White) ? 0 : 7;
    int kingCol = 4;
    int rookCol = kingside ? 7 : 0;

    // Check the hasMoved flag directly from the pieces
    const Piece& king = board[row][kingCol];
    const Piece& rook = board[row][rookCol];

    // Check if king has moved
    if (king.hasMoved) {
        std::cout << (color == Color::White ? "White" : "Black") << " king has already moved - cannot castle" << std::endl;
        return false;
    }

    // Check if rook has moved
    if (rook.type != PieceType::Rook || rook.hasMoved) {
        std::cout << (color == Color::White ? "White" : "Black") << " " << (kingside ? "kingside" : "queenside") << " rook has moved - cannot castle" << std::endl;
        return false;
    }

    // Check if king is in check
    if (isInCheck(color)) {
        std::cout << "Cannot castle while in check" << std::endl;
        return false;
    }

    // Check if path is clear and king doesn't pass through check
    int step = kingside ? 1 : -1;
    for (int col = kingCol + step; col != rookCol; col += step) {
        if (board[row][col].type != PieceType::None) {
            std::cout << "Path not clear for castling" << std::endl;
            return false;
        }
        if (isSquareAttacked({ col, row }, oppositeColor(color))) {
            std::cout << "King would pass through check - cannot castle" << std::endl;
            return false;
        }
    }

    // Check if destination square is attacked
    sf::Vector2i kingDestination = { kingCol + 2 * step, row };
    if (isSquareAttacked(kingDestination, oppositeColor(color))) {
        std::cout << "King would be in check after castling" << std::endl;
        return false;
    }

    return true;
}

bool ChessGame::isEnPassantValid(sf::Vector2i from, sf::Vector2i to) const {
    // Check if there's an en passant target set
    if (enPassantTarget.x == -1 || enPassantTarget.y == -1) {
        return false;
    }

    // Check if the destination matches the en passant target
    if (to != enPassantTarget) {
        return false;
    }

    // Make sure it's a pawn making the move
    const Piece& movingPawn = board[from.y][from.x];
    if (movingPawn.type != PieceType::Pawn) {
        return false;
    }

    // Make sure the move is diagonal (en passant capture pattern)
    if (abs(to.x - from.x) != 1) {
        return false;
    }

    // Check that there's an enemy pawn to capture
    // The captured pawn is on the same row as the moving pawn, at the target column
    int capturedPawnRow = from.y;
    if (!isInBounds({ to.x, capturedPawnRow })) {
        return false;
    }

    const Piece& capturedPawn = board[capturedPawnRow][to.x];
    if (capturedPawn.type != PieceType::Pawn || capturedPawn.color == movingPawn.color) {
        return false;
    }

    std::cout << "En passant is valid: capturing pawn at (" << to.x << "," << capturedPawnRow << ")" << std::endl;
    return true;
}



bool ChessGame::isInCheck(Color color) const {
    sf::Vector2i kingPos = findKing(color);
    if (kingPos.x == -1) return false; // King not found
    return isSquareAttacked(kingPos, oppositeColor(color));
}

bool ChessGame::isSquareAttacked(sf::Vector2i square, Color byColor) const {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece& piece = board[row][col];
            if (piece.type != PieceType::None && piece.color == byColor) {
                sf::Vector2i from = { col, row };

                // Special case for pawns (they attack diagonally)
                if (piece.type == PieceType::Pawn) {
                    int direction = (piece.color == Color::White) ? 1 : -1;
                    if (square.y == row + direction && abs(square.x - col) == 1) {
                        return true;
                    }
                }
                // For other pieces, use regular move validation (excluding check test)
                else {
                    bool validMove = false;
                    switch (piece.type) {
                    case PieceType::Rook:   validMove = isValidRookMove(from, square); break;
                    case PieceType::Bishop: validMove = isValidBishopMove(from, square); break;
                    case PieceType::Queen:  validMove = isValidQueenMove(from, square); break;
                    case PieceType::Knight: validMove = isValidKnightMove(from, square); break;
                    case PieceType::King:
                        validMove = (abs(square.x - col) <= 1 && abs(square.y - row) <= 1);
                        break;
                    default: break;
                    }
                    if (validMove) return true;
                }
            }
        }
    }
    return false;
}

sf::Vector2i ChessGame::findKing(Color color) const {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            const Piece& piece = board[row][col];
            if (piece.type == PieceType::King && piece.color == color) {
                return { col, row };
            }
        }
    }
    return { -1, -1 };
}

bool ChessGame::wouldBeInCheck(sf::Vector2i from, sf::Vector2i to, Color color) const {
    // Make temporary move
    Piece originalTarget = board[to.y][to.x];
    Piece movingPiece = board[from.y][from.x];

    // Handle en passant in simulation
    Piece capturedEnPassantPawn = { PieceType::None, Color::White };
    bool wasEnPassant = false;

    if (movingPiece.type == PieceType::Pawn && to == enPassantTarget && originalTarget.type == PieceType::None) {
        wasEnPassant = true;
        int capturedPawnRow = from.y;
        capturedEnPassantPawn = board[capturedPawnRow][to.x];
        const_cast<ChessGame*>(this)->board[capturedPawnRow][to.x] = { PieceType::None, Color::White };
    }

    // Simulate move
    const_cast<ChessGame*>(this)->board[to.y][to.x] = movingPiece;
    const_cast<ChessGame*>(this)->board[from.y][from.x] = { PieceType::None, Color::White };

    bool inCheck = isInCheck(color);

    // Restore board
    const_cast<ChessGame*>(this)->board[from.y][from.x] = movingPiece;
    const_cast<ChessGame*>(this)->board[to.y][to.x] = originalTarget;

    // Restore en passant captured pawn
    if (wasEnPassant) {
        int capturedPawnRow = from.y;
        const_cast<ChessGame*>(this)->board[capturedPawnRow][to.x] = capturedEnPassantPawn;
    }

    return inCheck;
}

std::vector<sf::Vector2i> ChessGame::getValidMoves(sf::Vector2i position) const {
    std::vector<sf::Vector2i> moves;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (isValidMove(position, { x, y })) {
                moves.emplace_back(x, y);
            }
        }
    }
    return moves;
}

std::vector<sf::Vector2i> ChessGame::getAllValidMoves(Color color) const {
    std::vector<sf::Vector2i> allMoves;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col].type != PieceType::None && board[row][col].color == color) {
                auto pieceMoves = getValidMoves({ col, row });
                allMoves.insert(allMoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    return allMoves;
}

void ChessGame::updateGameState() {
    bool inCheck = isInCheck(currentTurn);
    auto validMoves = getAllValidMoves(currentTurn);

    if (validMoves.empty()) {
        gameState = inCheck ? GameState::Checkmate : GameState::Stalemate;

        if (gameState == GameState::Checkmate) {
            std::cout << "\n*** CHECKMATE! " << (currentTurn == Color::White ? "Black" : "White") << " wins! ***\n";
        }
        else {
            std::cout << "\n*** STALEMATE! The game is a draw. ***\n";
        }
    }
    else {
        GameState previousState = gameState;
        gameState = inCheck ? GameState::Check : GameState::Playing;

        if (gameState == GameState::Check && previousState != GameState::Check) {
            std::cout << "\n*** CHECK! " << (currentTurn == Color::White ? "White" : "Black") << " king is in check! ***\n";
        }
    }
}

void ChessGame::performCastling(sf::Vector2i kingFrom, sf::Vector2i kingTo) {
    bool kingside = (kingTo.x > kingFrom.x);
    int row = kingFrom.y;
    int rookFromCol = kingside ? 7 : 0;
    int rookToCol = kingside ? 5 : 3;

    // Move rook
    board[row][rookToCol] = board[row][rookFromCol];
    board[row][rookFromCol] = { PieceType::None, Color::White };
    board[row][rookToCol].hasMoved = true;
    board[row][rookToCol].sprite.setPosition(rookToCol * 100 + 50, row * 100 + 50);

    std::cout << "Castling performed: " << (kingside ? "Kingside" : "Queenside") << std::endl;
}

void ChessGame::performEnPassant(sf::Vector2i from, sf::Vector2i to) {
    // The captured pawn is on the same rank as from.y, but the file is to.x
    int capturedPawnY = from.y;
    int capturedPawnX = to.x;

    // Remove the captured pawn
    board[capturedPawnY][capturedPawnX] = { PieceType::None, Color::White };
}

void ChessGame::movePiece(sf::Vector2i from, sf::Vector2i to) {
    std::cout << "Moving from (" << from.x << "," << from.y << ") to (" << to.x << "," << to.y << ")\n";
    Piece& movingPiece = board[from.y][from.x];
    bool isCapture = (board[to.y][to.x].type != PieceType::None);
   
    

    // Handle special moves
    bool isCastling = (movingPiece.type == PieceType::King && abs(to.x - from.x) == 2);
    bool isEnPassant = (movingPiece.type == PieceType::Pawn && to == enPassantTarget && board[to.y][to.x].type == PieceType::None);

    // Print move to console
    std::cout << "Move: " << (movingPiece.color == Color::White ? "White " : "Black ");
    switch (movingPiece.type) {
    case PieceType::King: std::cout << "King"; break;
    case PieceType::Queen: std::cout << "Queen"; break;
    case PieceType::Rook: std::cout << "Rook"; break;
    case PieceType::Bishop: std::cout << "Bishop"; break;
    case PieceType::Knight: std::cout << "Knight"; break;
    case PieceType::Pawn: std::cout << "Pawn"; break;
    default: break;
    }
    if (isCapture) {
        captureSound.play();
    }
    else {
        moveSound.play();
    }
    std::cout << " from (" << from.x << "," << from.y << ") to (" << to.x << "," << to.y << ")";
    if (board[to.y][to.x].type != PieceType::None) {
        std::cout << " (captures)";
        
    }
    if (isEnPassant) {
        std::cout << " (en passant)";
    }
    if (isCastling) {
        std::cout << " (castling)";
    }
    std::cout << std::endl;

    // Set hasMoved flag BEFORE executing the move
    movingPiece.hasMoved = true;

    // Handle en passant capture removal BEFORE moving the pawn
    if (isEnPassant) {
        int capturedPawnY = (movingPiece.color == Color::White) ? to.y - 1 : to.y + 1;
        board[capturedPawnY][to.x] = { PieceType::None, Color::White };
    }

    // Execute move
    board[to.y][to.x] = movingPiece;
    board[from.y][from.x] = { PieceType::None, Color::White };
    board[to.y][to.x].sprite.setPosition(to.x * 100 + 50, to.y * 100 + 50);

    // Handle special moves
    if (isEnPassant) {
        performEnPassant(from, to);
    }
    if (isCastling) {
        performCastling(from, to);
    }

    // Reset en passant target every move, then set new one if applicable
    enPassantTarget = { -1, -1 };
    if (movingPiece.type == PieceType::Pawn && abs(to.y - from.y) == 2) {
        enPassantTarget = { to.x, from.y + (to.y - from.y) / 2 };
        std::cout << "En passant target set at (" << enPassantTarget.x << "," << enPassantTarget.y << ")" << std::endl;
    }

    // If a pawn moved two squares, set en passant target
    if (movingPiece.type == PieceType::Pawn && abs(to.y - from.y) == 2) {
        // The en passant target is the square the pawn "jumped over"
        enPassantTarget = { to.x, from.y + (to.y - from.y) / 2 };
        std::cout << "En passant target set at (" << enPassantTarget.x << "," << enPassantTarget.y << ") - opponent can capture here next turn" << std::endl;
    }

    // Handle pawn promotion (simplified - always promote to queen)

    //rotate board autoupdate
    rotateBoard = (currentTurn == Color::Black);
    //
    
    Piece& movedPiece = board[to.y][to.x]; // Get piece at destination

    if (movedPiece.type == PieceType::Pawn) {
        if ((movedPiece.color == Color::White && to.y == 7) ||  // White promotes at bottom row
            (movedPiece.color == Color::Black && to.y == 0)) {  // Black promotes at top row

            movedPiece.type = PieceType::Queen;

            // Update sprite for promoted piece
            sf::Vector2u textureSize = piecesTexture.getSize();
            int pieceWidth = textureSize.x / 6;
            int pieceHeight = textureSize.y / 2;
            int spriteX = 3 * pieceWidth; // Queen sprite
            int spriteY = (movedPiece.color == Color::Black) ? 0 : pieceHeight;

            movedPiece.setTexture(piecesTexture, spriteX, spriteY, pieceWidth, pieceHeight);
            movedPiece.sprite.setPosition(to.x * 100 + 50, to.y * 100 + 50);

            std::cout << "*** PAWN PROMOTION! Promoted to Queen ***" << std::endl;
        }
    }

}

void ChessGame::handleMouseClick(sf::Vector2i mousePos) {
    //rotate board
    int x = mousePos.x / 100;
    int y = mousePos.y / 100;

    if (rotateBoard) {
        x = 7 - x;
        y = 7 - y;
    }

    sf::Vector2i boardPos(x, y);
    //
    if (menuState == MenuState::MainMenu) {
        handleMenuClick(mousePos);
        return;
    }

    //sf::Vector2i boardPos(mousePos.x / 100, mousePos.y / 100);

    if (!isInBounds(boardPos)) {
        isPieceSelected = false;
        return;
    }

    if (gameState == GameState::Checkmate || gameState == GameState::Stalemate) {
        return; // Game is over
    }

    if (!isPieceSelected) {
        if (board[boardPos.y][boardPos.x].type != PieceType::None &&
            board[boardPos.y][boardPos.x].color == currentTurn) {
            selectedPosition = boardPos;
            isPieceSelected = true;

            std::cout << "Selected " << (currentTurn == Color::White ? "White " : "Black ");
            switch (board[boardPos.y][boardPos.x].type) {
            case PieceType::King: std::cout << "King"; break;
            case PieceType::Queen: std::cout << "Queen"; break;
            case PieceType::Rook: std::cout << "Rook"; break;
            case PieceType::Bishop: std::cout << "Bishop"; break;
            case PieceType::Knight: std::cout << "Knight"; break;
            case PieceType::Pawn: std::cout << "Pawn"; break;
            default: break;
            }
            std::cout << " at (" << boardPos.x << "," << boardPos.y << ")";
            if (board[boardPos.y][boardPos.x].hasMoved) {
                std::cout << " (has moved)";
            }
            else {
                std::cout << " (hasn't moved)";
            }
            std::cout << std::endl;

            // Show en passant info
            if (enPassantTarget.x != -1) {
                std::cout << "En passant target available at (" << enPassantTarget.x << "," << enPassantTarget.y << ")" << std::endl;
            }
        }
    }
    else {
        if (isValidMove(selectedPosition, boardPos)) {
            movePiece(selectedPosition, boardPos);
            currentTurn = oppositeColor(currentTurn);
            updateGameState();

            std::cout << "Turn: " << (currentTurn == Color::White ? "White" : "Black") << std::endl;
        }
        isPieceSelected = false;
    }
}

void ChessGame::drawBoard() {
    sf::RectangleShape square(sf::Vector2f(100, 100));

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Calculate draw positions with rotation
            int drawX = rotateBoard ? 7 - col : col;
            int drawY = rotateBoard ? 7 - row : row;

            square.setPosition(drawX * 100, drawY * 100);
            bool isLightSquare = (col + row) % 2 == 0;
            square.setFillColor(isLightSquare ? sf::Color(240, 217, 181) : sf::Color(181, 136, 99));
            window.draw(square);
        }
    }

    // Highlight en passant target square (apply rotation)
    if (enPassantTarget.x != -1 && enPassantTarget.y != -1) {
        sf::RectangleShape enPassantHighlight(sf::Vector2f(100, 100));
        int drawX = rotateBoard ? 7 - enPassantTarget.x : enPassantTarget.x;
        int drawY = rotateBoard ? 7 - enPassantTarget.y : enPassantTarget.y;
        enPassantHighlight.setPosition(drawX * 100, drawY * 100);
        enPassantHighlight.setFillColor(sf::Color(0, 0, 255, 64)); // Blue highlight
        window.draw(enPassantHighlight);
    }
   }


void ChessGame::drawPieces() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col].type != PieceType::None) {
                window.draw(board[row][col].sprite);
            }
        }
    }
    //rotate board
     for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board[y][x].type != PieceType::None) {
                int drawX = rotateBoard ? 7 - x : x;
                int drawY = rotateBoard ? 7 - y : y;
                board[y][x].sprite.setPosition(drawX * 100 + 50, drawY * 100 + 50);
                window.draw(board[y][x].sprite);
            }
        }
    }
}

void ChessGame::drawSelection() {
    
    if (isPieceSelected) {
        // board rotation 
        int drawX = rotateBoard ? 7 - selectedPosition.x : selectedPosition.x;
        int drawY = rotateBoard ? 7 - selectedPosition.y : selectedPosition.y;

        // Draw green dot highlight for selected square
        sf::CircleShape dotHighlight(10);
        dotHighlight.setFillColor(sf::Color::Green);
        dotHighlight.setPosition(drawX * 100 + 40, drawY * 100 + 40);
        window.draw(dotHighlight);

        // Draw yellow rectangle highlight for selected piece
        sf::RectangleShape rectHighlight(sf::Vector2f(100, 100));
        rectHighlight.setFillColor(sf::Color(255, 255, 0, 128));
        rectHighlight.setPosition(drawX * 100, drawY * 100);
        window.draw(rectHighlight);

        // Highlight valid moves with circles
        auto validMoves = getValidMoves(selectedPosition);
        for (const auto& move : validMoves) {
            int moveX = rotateBoard ? 7 - move.x : move.x;
            int moveY = rotateBoard ? 7 - move.y : move.y;

            sf::CircleShape moveCircle(20);
            moveCircle.setFillColor(sf::Color(0, 255, 0, 128));
            moveCircle.setPosition(moveX * 100 + 30, moveY * 100 + 30);
            window.draw(moveCircle);
        }
    }


    // Highlight king if in check
    if (gameState == GameState::Check || gameState == GameState::Checkmate) {
        sf::Vector2i kingPos = findKing(currentTurn);
        if (kingPos.x != -1) {
            sf::RectangleShape checkHighlight(sf::Vector2f(100, 100));
            checkHighlight.setPosition(kingPos.x * 100, kingPos.y * 100);
            checkHighlight.setFillColor(sf::Color(255, 0, 0, 128));
            window.draw(checkHighlight);
        }
    }
}

void ChessGame::run() {
    std::cout << "\n=== SFML Chess Game Started ===\n";
    std::cout << "Navigate the main menu with UP/DOWN arrows and ENTER to select.\n";
    std::cout << "You can also click on menu items with the mouse.\n";
    std::cout << "If you see rectangles instead of text, that means no font was loaded.\n\n";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    handleMouseClick({ event.mouseButton.x, event.mouseButton.y });
                }
            }
        }

        window.clear();

        if (menuState == MenuState::MainMenu) {
            drawMenu();
        }
        else {
            drawBoard();
            drawSelection();
            drawPieces();
        }

        window.display();
    }
}

bool ChessGame::isInBounds(sf::Vector2i pos) const {
    return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

Color ChessGame::oppositeColor(Color color) const {
    return (color == Color::White) ? Color::Black : Color::White;
}

void ChessGame::undoMove(const Move& move) {
    // Implementation for undo functionality (last moment bycott)
}

//rotate board every move
void ChessGame::switchTurn() {
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
    rotateBoard = !rotateBoard;
}
