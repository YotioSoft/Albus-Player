#include "AlbusBox.hpp"

pair<bool, FilePath> FileOpen() {
	// ファイル選択ダイアログ
	Array<FileFilter> ff = { {U"音声ファイル", {U"mp3", U"ogg", U"wav", U"m4a"}} };
	String filePathTemp;
	if (const auto open = Dialog::OpenFile(ff)) {
		return pair<bool, FilePath>(true, open.value());
	}

	return pair<bool, FilePath>(false, FilePath());
}

bool ExitButton(Font& font16, Color& button_close_color, Texture& window_close_icon) {
	// ウィンドウを閉じるボタン
	if (OS == "Windows") {
		if (NeumorphismUI::CircleButton(Scene::Width()-30, 30, 15, window_close_icon)) {
			return true;
		}
	}
	else if (OS == "Mac") {
		if (NeumorphismUI::CircleButton(25, 25, 8, U"", font16, button_close_color)) {
			return true;
		}
	}
	
	return false;
}

void exit(Array<Audio>& audio_files_list, int track_in_list) {
	audio_files_list[track_in_list].stop();

	for (auto af : audio_files_list)
		af.release();
	
	System::Exit();
}

bool VersionInformation(Player& player, Font& font13, Font& font16B, Font& font16, Font& font36,
	Color& button_close_color, Texture& window_close_icon, Color& font_color) {
	// ボタン用アイコン
	Texture return_button_icon(Icon(IconFont::Return, 20));		// 戻る

	// 画像
	Texture albus_box_icon(U"./data/img/icon.png");

	// ボタンの位置
	Point return_button_pos;
	if (OS == "Windows") {
		return_button_pos = Point(10, 10);
	}
	else if (OS == "Mac") {
		return_button_pos = Point(45, 10);
	}

	// マウスクリックした地点の記録用
	Point mouse_clicked;
	bool window_moving = false;

	while (System::Update()) {
		// 画面上部のボタン群
		// 閉じるボタン
		if (ExitButton(font16B, button_close_color, window_close_icon)) {
			return true;
		}
		// もどる
		if (NeumorphismUI::RectButton(return_button_pos, Vec2(40, 40), return_button_icon)) {
			return false;
		}

		// 画面タイトル
		font16B(U"バージョン情報").draw(Arg::center(Scene::Width() / 2, 30), Color(font_color));

		// バージョン情報
		albus_box_icon.resized(200, 200).drawAt(Scene::Width() / 2, 200);

		font36(U"Albus Box").drawAt(Scene::Width() / 2, 350, font_color);

		font16(U"Version: {:>24}\nRelease Number: {:>8}"_fmt(VERSION, RELEASE_NUM)).drawAt(Scene::Width() / 2, 450, font_color);

		font13(U"©YotioSoft 2021 | Powered by OpenSiv3D").drawAt(Scene::Width() / 2, Scene::Height()-50, font_color);

		// 再生処理を継続
		player.playing();

		// ウィンドウの移動
		if (MouseL.down()) {
			mouse_clicked = Cursor::Pos();
			window_moving = true;
		}
		else if (MouseL.pressed() && Cursor::GetRequestedStyle() == CursorStyle::Arrow && window_moving) {
			Window::SetPos(Cursor::ScreenPos() - mouse_clicked);
		}
		else {
			window_moving = false;
		}
	}
	return true;
}

bool AlbusBoxSetting(Player& player, Font& font13, Font& font16B, Font& font16, Font& font36,
	Color& button_close_color, Texture& window_close_icon, Color& font_color) {
	// ボタン用アイコン
	Texture return_button_icon(Icon(IconFont::Return, 20));		// 戻る
	
	// アイコン
	Texture volume_icon(Icon(IconFont::Volume, 30));
	Texture info_icon(Icon(IconFont::Information, 30));

	// 画像
	Texture albus_box_icon(U"./data/img/icon.png");
	
	// アイコンの位置
	const int icon_left_x = 50;
	const int icon_top_y = 100;
	
	// 音量調整バー
	double volume = player.getVolume();
	double volume_before = volume;
	NeumorphismUI::Slider volume_bar(volume, Vec2{ icon_left_x+100, icon_top_y }, Scene::Width()-icon_left_x*2-100, 30);

	// スイッチ
	// 波形の表示
	NeumorphismUI::SlideSwitch show_wave_switch(player.isShowWaveEnabled(), Vec2(Scene::Width() - icon_left_x - 50, icon_top_y + 100), Vec2(50, 30));
	// ループ再生
	NeumorphismUI::SlideSwitch loop_switch(player.isLoopEnabled(), Vec2(Scene::Width() - icon_left_x - 50, icon_top_y + 180), Vec2(50, 30));
	
	// ボタンの位置
	Point return_button_pos;
	if (OS == "Windows") {
		return_button_pos = Point(10, 10);
	}
	else if (OS == "Mac") {
		return_button_pos = Point(45, 10);
	}
	
	// マウスクリックした地点の記録用
	Point mouse_clicked;
	bool window_moving = false;
	
	while (System::Update()) {
		// 画面上部のボタン群
		// 閉じるボタン
		if (ExitButton(font16B, button_close_color, window_close_icon)) {
			return true;
		}
		// もどる
		if (NeumorphismUI::RectButton(return_button_pos, Vec2(40, 40), return_button_icon)) {
			return false;
		}
		
		// 画面タイトル
		font16B(U"設定").draw(Arg::center(Scene::Width()/2, 30), Color(font_color));
		
		// 各設定項目
		// 音量
		volume_icon.draw(icon_left_x, icon_top_y, Color(font_color));
		volume = volume_bar.draw();
		if (volume != volume_before) {
			volume_before = volume;
			player.changeVolumeTo(volume);
		}

		// 波形の表示
		font16(U"波形の表示").draw(icon_left_x, icon_top_y + 100, font_color);
		player.setShowWave(show_wave_switch.draw());

		// ループ再生
		font16(U"ループ再生").draw(icon_left_x, icon_top_y + 180, font_color);
		player.setLoop(loop_switch.draw());

		// バージョン情報
		if (NeumorphismUI::RectButton(Vec2(Scene::Width() / 2 - 50/2, Scene::Height() - 100), Vec2(50, 50), info_icon)) {
			if (VersionInformation(player, font13, font16B, font16, font36, button_close_color, window_close_icon, font_color)) {
				return true;
			}
		}

		// 再生処理を継続
		player.playing();
		
		// ウィンドウの移動
		if (MouseL.down()) {
			mouse_clicked = Cursor::Pos();
			window_moving = true;
		}
		else if (MouseL.pressed() && Cursor::GetRequestedStyle() == CursorStyle::Arrow && window_moving) {
			Window::SetPos(Cursor::ScreenPos() - mouse_clicked);
		}
		else {
			window_moving = false;
		}
	}
	return true;
}

void AlbusBox() {
	Scene::SetBackground(DEFAULT_BACKGROUND_COLOR);
	Window::SetStyle(WindowStyle::Frameless);
	Window::Resize(400, 640);

	specific::setWindowStyle(0, 0, 400, 640, 40, 40);

	Font font13(13, U"{}/NotoSansCJKjp/NotoSansCJKjp-Regular.otf"_fmt(specific::getFontsDir()));
	Font font16(16, U"{}/NotoSansCJKjp/NotoSansCJKjp-Regular.otf"_fmt(specific::getFontsDir()));
	Font font16B(16, U"{}/NotoSansCJKjp/NotoSansCJKjp-Bold.otf"_fmt(specific::getFontsDir()));
	Font font36(36, U"{}/NotoSansCJKjp/NotoSansCJKjp-Regular.otf"_fmt(specific::getFontsDir()));
	
	specific::moveToCurrentDir();

	// シークバー
	double play_pos = 0.0;		// シークバーの初期値
	int slider_width = Scene::Width() - 100;
	NeumorphismUI::Slider slider(play_pos, Vec2{ 50, Scene::Height() / 2 + 100 }, Scene::Width() - 100, 30);

	bool playing = false;	// 再生中
	bool before_playing = playing;

	// マウスクリックした地点の記録用
	Point mouse_clicked;
	bool window_moving = false;

	// アイコン
	Texture play_icon(Icon(IconFont::Play, 30));
	Texture pause_icon(Icon(IconFont::Pause, 30));

	Texture prev_icon(Icon(IconFont::Prev, 20));
	Texture next_icon(Icon(IconFont::Next, 20));

	Texture setting_icon(Icon(IconFont::Setting, 20));
	Texture fileopen_icon(Icon(IconFont::FileOpen, 20));

	Texture window_close_icon(Icon(IconFont::Times, 20));

	// ボタンの位置
	Point setting_button_pos;
	Point fileopen_button_pos;
	if (OS == "Windows") {
		setting_button_pos = Point(30, 30);
		fileopen_button_pos = Point(80, 30);
	}
	else if (OS == "Mac") {
		fileopen_button_pos = Point(Scene::Width() - 80, 30);
		setting_button_pos = Point(Scene::Width() - 30, 30);
	}

	// ボタン押下検知用
	bool prev_button_pressed = false;
	bool next_button_pressed = false;

	// 色
	Color font_color(Palette::Gray);
	Color button_close_color(Color(242, 38, 19));

	// サムネ用
	const int thumbnail_size = 260;
	Image thumbnail_image(thumbnail_size, thumbnail_size, Palette::White);
	Color thumbnail_color1(Palette::Orangered);
	Color thumbnail_color2(Palette::Mediumpurple);

	for (int y = 0; y < thumbnail_image.height(); y++) {
		for (int x = 0; x < thumbnail_image.width(); x++) {
			double t = (double)(x + y) / (thumbnail_image.width() + thumbnail_image.height() - 2);
			thumbnail_image[y][x] = thumbnail_color1.lerp(thumbnail_color2, t);
		}
	}
	RenderTexture thumbnail_texture(thumbnail_image);
	Texture thunbnail_image_texture(thumbnail_image);
	Circle thumbnail_circle(Scene::Width() / 2, Scene::Height() / 3, thumbnail_size / 2);

	//Wave wave = audio_file.getWave();
	//cout << wave.size() << " " << audio_file.samples() << endl;
	/*
	for (int t=0; t<wave.size(); t++) {
		wave[t].left = 0.3;
		wave[t].right = 0.3;
	}
	audio_file = Audio(wave);
	*/

	FFTResult fft;
	
	// プレイヤーの用意
	Player player;

	// 座標変換行列（タイトル用）
	Mat3x2 mat = Mat3x2::Identity();
	RectF title_rect, title_rect_before;
	int count_for_music = 0;

	// タイトル編集用
	TextEditState tes_title;
	bool editing_title = false;

	while (System::Update()) {
		// 画面上部のボタン群
		bool isExitButtonPushed = ExitButton(font16, button_close_color, window_close_icon);
		if (isExitButtonPushed)
			break;

		// 画面上部のボタン群
		// 設定へ画面遷移
		if (NeumorphismUI::CircleButton(setting_button_pos, 20, setting_icon)) {
			if (AlbusBoxSetting(player, font13, font16B, font16, font36, button_close_color, window_close_icon, font_color)) {
				break;		// 閉じるボタンが押されたらループを抜ける
			}
		}
		
		// ファイルを開く
		if (NeumorphismUI::CircleButton(fileopen_button_pos, 20, fileopen_icon)) {
			auto file_open = FileOpen();
			if (file_open.first) {
				player.open(file_open.second);
			}
		}

		// サムネイル
		NeumorphismUI::NeumorphismCircle(Scene::Width() / 2, Scene::Height() / 3, thumbnail_size / 2 + 10, false);

		// 波形を表示
		LineString fft_line;
		if (playing && player.isOpened() && player.isShowWaveEnabled()) {
			ScopedRenderTarget2D target(thumbnail_texture);
			thunbnail_image_texture.draw(0, 0);
			player.fft(fft);
			int fft_size = 800;
			int box_size = 10;
			int row_boxes = 20;
			for (int i = 0; i < row_boxes; i++) {
				double size = Pow(fft.buffer[i * (double)fft_size / row_boxes], 0.6f) * 2000;
				int fft_box_count = size / box_size;
				for (int j = 0; j < fft_box_count; j++) {
					Rect(Arg::center(i * thumbnail_texture.width() / row_boxes + box_size / 2, thumbnail_texture.height() - j * box_size - box_size / 2), box_size * 3 / 4, box_size * 3 / 4).draw(Color(200, 200, 200, 200));
				}
			}
		}
		else {
			ScopedRenderTarget2D target(thumbnail_texture);
			thunbnail_image_texture.draw(0, 0);
		}

		thumbnail_circle(thumbnail_texture(0, 0, thumbnail_size, thumbnail_size)).draw();

		// タイトル
		// タイトル部分のマウスオーバー時にタイトル部分を光らせる
		if (title_rect.mouseOver()) {
			title_rect.draw(Color(255, 255, 255, 100));
		}

		// タイトルの表示
		title_rect = font16(player.getTitle()).region(Arg::center(Scene::Width() / 2, Scene::Height() / 3 + 170));
		if (title_rect != title_rect_before) {
			title_rect_before = title_rect;
			count_for_music = Scene::Width();
		}
		if (title_rect.w > Scene::Width() && !editing_title) {
			mat = Mat3x2::Translate((title_rect.w - Scene::Width()) / 2 - (count_for_music % ((int)title_rect.w + Scene::Width())) + Scene::Width(), 0);
			{
				const Transformer2D t(mat, false);
				font16(player.getTitle()).drawAt(Scene::Width() / 2, Scene::Height() / 3 + 170, font_color);
			}
		}
		else if (!editing_title) {
			font16(player.getTitle()).drawAt(Scene::Width() / 2, Scene::Height() / 3 + 170, font_color);
		}
		count_for_music++;

		// タイトル部分がクリックされたらテキストボックスを設置
		if (title_rect.leftClicked() && !editing_title) {
			editing_title = true;
			tes_title.text = player.getTitle();
		}
		if (editing_title) {
			SimpleGUI::TextBox(tes_title, Vec2(0, Scene::Height() / 3 + 170 - 10), Scene::Width());

			// editing_title == trueかつタイトル部分以外でクリックされたら編集完了
			if (!title_rect.leftClicked() && MouseL.down()) {
				editing_title = false;
				player.editTitle(tes_title.text);
			}
		}
		// editing_title == trueかつタイトル部分以外でクリックされたら編集完了
		if (editing_title && !title_rect.leftClicked() && MouseL.down()) {
			editing_title = false;
		}

		// シークバー
		if (!slider.isSliderMoving()) {
			slider.setValueNoAnimetion(player.getPlayPosNorm());
			//cout << (double)audio_file.posSample()/audio_file.samples() << endl;
		}
		play_pos = slider.draw();
		if (slider.isSliderLeftReleased()) {
			player.seekTo(play_pos);
		}

		// 再生位置表示
		font13(U"{}:{:0>2}"_fmt(player.getPlayPosTimeMin(), player.getPlayPosTimeSec())).draw(40, Scene::Height() / 2 + 140, font_color);
		font13(U"{}:{:0>2}"_fmt(player.getTotalTimeMin(), player.getTotalTimeSec())).draw(Arg::topRight = Point(Scene::Width() - 40, Scene::Height() / 2 + 140), font_color);

		// トラック移動ボタン
		prev_button_pressed = NeumorphismUI::CircleButton(Scene::Width() / 4 - 10, Scene::Height() - 100, 30, prev_icon);
		next_button_pressed = NeumorphismUI::CircleButton(Scene::Width() - Scene::Width() / 4 + 10, Scene::Height() - 100, 30, next_icon);
		
		if (prev_button_pressed)
			player.previous();
		if (next_button_pressed)
			player.next();

		// 再生/一時停止ボタン
		NeumorphismUI::CircleSwitch(Scene::Width() / 2, Scene::Height() - 100, 40, playing, play_icon, pause_icon);
		if (before_playing != playing) {
			if (playing)
				player.play();
			else
				player.pause();
			
			before_playing = playing;
		}
		
		playing = player.playing();
		
		// ウィンドウの移動
		if (MouseL.down()) {
			mouse_clicked = Cursor::Pos();
			window_moving = true;
		}
		else if (MouseL.pressed() && Cursor::GetRequestedStyle() == CursorStyle::Arrow && window_moving) {
			Window::SetPos(Cursor::ScreenPos() - mouse_clicked);
		}
		else {
			window_moving = false;
		}
	}
}
