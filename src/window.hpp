#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

// TODO include into a namespace?
#include <ncurses.h>

// TODO better trickery here?
class Window;
#include "cursor.hpp"

// TODO guh, better way to do this?
namespace Key {
    enum Key { Escape = 27,
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
        Delete = 127 };
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
        /// Updates the window
        int update();

        /// Returns the next input key
        int getKey();

        /// Write a string where the internal cursor is at
        bool write(std::string s);
        /// Writes a string to where a Cursor is at
        bool write(Cursor *c, std::string s);
        /// Writes a four width int to a specified position
        bool write(Cursor *c, int i);

        /// Returns the width of the window
        int getWidth() const;
        /// Returns the height of the window
        int getHeight() const;

        /// Returns a pointer to this's internal cursor
        Cursor *getCursor();

    protected:
        // todo: implement these, or not?
        Window(const Window &rhs);
        Window &operator=(const Window &rhs);

        bool cursesMode;
        Cursor *cursor;
};

#endif // WINDOW_HPP
// vim:ts=4 et sw=4 sts=4
