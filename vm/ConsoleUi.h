#pragma once
#include <Windows.h>
#include "cui_common.h"

namespace cui {

    class ConsoleUi;
    class ConsoleDC {
    public:
        Rect m_limit;
        CharGrid m_grid;
        HANDLE m_std;

        ConsoleDC(const Rect& rect, HANDLE handle)
            : m_limit(rect), m_grid(rect),
              m_std(handle) {}

        bool testcoord(SHORT x, SHORT y) {
            return m_limit.hittest(x, y);
        }
        bool testcoord(COORD xy) {
            return m_limit.hittest(xy);
        }
        RECT_NUM_TYPE getx() const { return m_limit.m_x; }
        RECT_NUM_TYPE gety() const { return m_limit.m_y; }
        RECT_NUM_TYPE getw() const { return m_limit.m_width; }
        RECT_NUM_TYPE geth() const { return m_limit.m_height; }
        bool testrect(Rect r) {
            auto tl = r.topleft(), br = r.botright();
            return m_limit.hittest(tl) && m_limit.hittest(br);
        }
        void border() {
            m_grid.set(getx(), gety(), BoxChars::TOP_LEFT);
            for(int x=getx();x<x+getw();x++) {
                for(int y=gety();y<y+geth();y++) {
                    
                }
            }
        }
        void flush() {
            auto sr = m_limit.region();
            auto flat = m_grid.flatten();
            WriteConsoleOutput(m_std, flat, 
                { m_limit.m_width, m_limit.m_height }, { 0,0 }, &sr);
            delete flat;
        }
    };
    class IControl {
    public:
        ConsoleUi* m_context;
        HANDLE m_std;
        Rect m_rect;
        bool m_valid;

        explicit IControl(ConsoleUi* console_ui)
            : m_context(console_ui), m_rect(), m_valid(true) {
            m_std = GetStdHandle(STD_OUTPUT_HANDLE);
        }
        void invalidate() { m_valid = false; }
        void setrect(Rect r) {
            m_rect = r;
            invalidate();
        }
        void setrect(SMALL_RECT sr) {
            m_rect = sr;
            invalidate();
        }
        virtual void paint(ConsoleDC* dc) {

        }
    };

    class ConsoleUi
    {
    public:
        ConsoleUi();
        ~ConsoleUi();
    };
}
