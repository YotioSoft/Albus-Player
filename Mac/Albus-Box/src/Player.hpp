//
//  Player.hpp
//  Albus-Box
//
//  Created by YotioSoft on 2021/08/07.
//

#ifndef Player_hpp
#define Player_hpp

#include <Siv3D.hpp>

namespace PlayerStatus {
	enum Type {
		Stop	= -1,
		Pause	= 0,
		Play	= 1
	};
}

class Player {
public:
	// コンストラクタ
	Player();
	
	// 曲を開く
	void open(FilePath audio_filepath);
	
	// ループの設定
	void setLoop(bool enable);
	
	// 再生
	bool play();
	bool playFromBegin();
	
	// 再生中の処理
	bool playing();
	
	// 一時停止
	bool pause();
	
	// 再生停止
	bool stop();
	
	// 前の曲へ
	void previous();
	
	// 次の曲へ
	void next();
	
	// 再生位置変更
	bool seekTo(double skip_pos);
	
	// 現在の音量を返す
	double getVolume();
	
	// 音量変更
	bool changeVolumeTo(double volume_norm);
	
	// 曲のタイトルを返す
	String getTitle();
	
	// 再生位置を返す(秒)
	int getPlayPosSec();
	
	// 時分で再生位置を返す
	int getPlayPosTimeMin();
	int getPlayPosTimeSec();
	
	// 再生位置を返す(0.0~1.0)
	double getPlayPosNorm();
	
	// 再生位置を返す(フレーム)
	int64 getPlayPosSample();
	
	// 時分で曲の長さを返す
	int getTotalTimeMin();
	int getTotalTimeSec();
	
	// 高速フーリエ変換
	void fft(FFTResult& fft);
	
	// ファイルが開かれているか否か
	bool isOpened();
	
	// 解放
	void free();
	
private:
	Array<Audio*> audio_files;
	Array<FilePath> audio_files_path;
	int current_track;
	PlayerStatus::Type status;
	double volume;
};

#endif /* Player_hpp */
