#pragma once
#include <Windows.h>
namespace cui {
    class BoxChars {
    public:
#define CHARDEF(NAME, val) static const unsigned char NAME = val;
        CHARDEF(VERTICAL, 0xB3);
        CHARDEF(HORIZONTAL, 0xC4);
        CHARDEF(TOP_LEFT, 0xDA);
        CHARDEF(TOP_RIGHT, 0xBF);
        CHARDEF(BOT_LEFT, 0xC0);
        CHARDEF(BOT_RIGHT, 0xD9);
    };
#define RECT_NUM_TYPE SHORT
    class Rect {
    public:
        RECT_NUM_TYPE m_x, m_y, m_width, m_height;
        Rect() :m_x(0), m_y(0), m_width(0), m_height(0) {}
        Rect(RECT_NUM_TYPE x, RECT_NUM_TYPE y, RECT_NUM_TYPE w, RECT_NUM_TYPE h)
            : m_x(x),
            m_y(y),
            m_width(w),
            m_height(h) {}

        void setrect(const SMALL_RECT& sr) {
            m_x = sr.Left;
            m_y = sr.Top;
            m_width = sr.Right - sr.Left;
            m_height = sr.Bottom - sr.Top;
        }

        explicit Rect(SMALL_RECT sr) {
            setrect(sr);
        }

        Rect(const Rect& other)
            : m_x(other.m_x),
            m_y(other.m_y),
            m_width(other.m_width),
            m_height(other.m_height) {}

        Rect(Rect&& other) noexcept
            : m_x(other.m_x),
            m_y(other.m_y),
            m_width(other.m_width),
            m_height(other.m_height) {}

        Rect& operator=(const Rect& other) {
            if (this == &other)
                return *this;
            m_x = other.m_x;
            m_y = other.m_y;
            m_width = other.m_width;
            m_height = other.m_height;
            return *this;
        }

        Rect& operator=(Rect&& other) noexcept {
            if (this == &other)
                return *this;
            m_x = other.m_x;
            m_y = other.m_y;
            m_width = other.m_width;
            m_height = other.m_height;
            return *this;
        }

        COORD getSize() {
            //The size of the buffer pointed to by the lpBuffer parameter, 
            //in character cells. The X member of the COORD structure is 
            //the number of columns; the Y member is the number of rows.
            return{ m_width, m_height };
        }
        COORD topleft() {
            return{ m_x,m_y };
        }
        COORD botright() {
            return{ m_x + m_width,m_y + m_height };
        }
        SMALL_RECT region() {
            SMALL_RECT r;
            r.Top = m_y;
            r.Left = m_x;
            r.Right = m_x + m_width;
            r.Bottom = m_y + m_height;
            return r;
        }
        bool hittest(COORD xy) {
            return hittest(xy.X, xy.Y);
        }
        bool hittest(RECT_NUM_TYPE x, RECT_NUM_TYPE y) {
            auto reg = region();
            return (x >= reg.Left && x <= reg.Right) &&
                (y >= reg.Top && y <= reg.Bottom);
        }
        void shrink(int amt) {
            shrink(amt, amt);
        }
        void shrink(int xamt, int yamt) {
            m_x += xamt;
            m_width -= xamt;
            m_y -= yamt;
            m_height -= yamt;
        }
        Rect& operator=(const SMALL_RECT& sr) {
            setrect(sr);
            return *this;
        }
    };
    class CharGrid {
    public:
        Rect m_rect;
        CHAR_INFO** m_infos;
        void init() {
            const int w = m_rect.m_width, h = m_rect.m_height;
            m_infos = new CHAR_INFO*[w];
            for(int x=0;x<w;x++) {
                m_infos[x] = new CHAR_INFO[h];
                for (int y = 0; y < h; y++)
                    m_infos[x][y] = CHAR_INFO();
            }
        }
        explicit CharGrid(const Rect& rect)
            : m_rect(rect) {
            init();
        }
        void set(int x, int y, unsigned char c, int ct = 1) {
            if (!m_rect.hittest(x, y))return;
            if (!m_rect.hittest(x+ct-1, y))return;
            for (int i = 0; i < ct; i++)
                m_infos[x + i][y].Char.AsciiChar = c;
        }
        CHAR_INFO& at(int x, int y) {
            if (!m_rect.hittest(x, y))return CHAR_INFO();
            return m_infos[x][y];
        }
        const CHAR_INFO *flatten() {
            auto ci = new CHAR_INFO[m_rect.m_width*m_rect.m_height];
            for (int x = m_rect.m_x; x<m_rect.m_x + m_rect.m_width; x++) {
                for (int y = m_rect.m_y; y < m_rect.m_y + m_rect.m_height; y++) {
                    ci[x, y] = m_infos[x][y];
                }
            }
            return ci;
        }
    };
}