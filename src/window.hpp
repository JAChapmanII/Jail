#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

// TODO include into a namespace?
#include <ncurses.h>

// TODO guh, better way to do this?
// TODO move out of here?
namespace Key {
    enum Key {
        CtrlB = 2, CtrlF = 6, Enter = '\n',
        Escape = 27,
        Up = KEY_UP, Down = KEY_DOWN, Left = KEY_LEFT, Right = KEY_RIGHT,
        Space = ' ', ExclamationPoint, QuotationMark, Octothorpe, Currency,
        Modulus, Ampersand, Apostraphe, OpenParenthesis, CloseParenthesis,
        Asterisk, Plus, Minus, Period, Slash,
        Zero = '0', One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
        Colon, Semicolon, LessThan, Equals, GreaterThan, QuestionMark, At,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V,
        W, X, Y, Z, OpenSquareBracket, Backslash, CloseSquareBracket, Caret,
        Underscore, Grave, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,
        q, r, s, t, u, v, w, x, y, z, OpenBrace, Pipe, CloseBrace, Tilde,
        // TODO fix these
        Delete = 127, Backspace = 127,
        PageDown = 338, PageUp };
}

/**
 * An OOP ncurses interface class which represents a window.
 */
class Window {
    public:
        /// Construct the default window, one which uses the entire screen
        Window();
        /// Proper deconstructor for the Window
        ~Window();

        /// Start ncurses mode
        void start();
        /// Puts the backing screen into regular mode
        void stop();
        /// Updates the window, moves cursor to set position
        int update(int x, int y);

        /// Returns the next input key
        int getKey();

        /// Write a string where the internal cursor is at
        bool write(std::string s);
        /// Writes a string at a specified row
        bool write(int wRow, std::string s);

        /// Returns the width of the window
        int getWidth() const;
        /// Returns the height of the window
        int getHeight() const;

        /// Set the internal cursor's row
        void setRow(int nRow);

    protected:
        // todo: implement these, or not?
        Window(const Window &rhs);
        Window &operator=(const Window &rhs);

        bool cursesMode;
        int row;
};

#endif // WINDOW_HPP
// vim:ts=4 et sw=4 sts=4
