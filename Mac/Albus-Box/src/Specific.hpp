//
//  Specific.hpp
//  Cities Box
//
//  注 : これはmacOS向けです
//
//  Created by YotioSoft on 2019/12/01.
//

#ifndef Specific_h
#define Specific_h

#include <unistd.h>
#include <dirent.h>
#include <random>
#include <iostream>
#include <filesystem>

//#include <Siv3D/Window.hpp>

#include <OtherFunctions.hpp>

/* OS依存の定数など */
#define LINE_FEED_CODE			0		// 改行コードに用いる文字数

#define WINDOW_WIDTH			400		// ウィンドウの幅
#define WINDOW_HEIGHT			640		// ウィンドウの高さ
#define WINDOW_ROUNDRECT_RADIUS	40		// ウィンドウの角の丸みの半径

namespace specific {
	String getCurrentDir();
	String getFontsDir();

	bool isCursorEntered(Vec2 before_cursor_pos);
	void changeCursor();

	String getSettingFilePath();
	String getAudioProfilesFilePath();
	String getLyricsDirPath();
	
	double getDpiDist();
	bool setWindowStyle(int x1, int y1, int x2, int y2, int w, int h);

	void moveWindow(Point& mouse_clicked, bool& window_moving);
	void pinWindow(bool temp);
}

#endif /* Specific_h */
