#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <DirectXMath.h>
#include <memory>

#include "PmxModel.h"

using namespace DirectX;

class PmxLoader
{
private: // エイリアス
	// std::を省略
	using string = std::string;
	using wstring = std::wstring;
	using ifstream = std::ifstream;

public: // 定数
	// モデル格納ルートパス
	static const wstring baseDirectory;

public:
	// シングルトンインスタンスの取得
	static PmxLoader* GetInstance();
	// PMXモデルデータのロード関数
	std::unique_ptr<PmxModel> loadPMX(const wstring& _filePath);

private:
	// privateなコンストラクタ（シングルトンパターン）
	PmxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~PmxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	PmxLoader(const PmxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const PmxLoader& obj) = delete;
	bool GetPMXStringUTF16(ifstream& _file, wstring& output);
};