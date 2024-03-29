#include "Model.h"

#include <fstream>
#include <sstream>

using namespace std;

// 静的メンバ変数の実体
ID3D12Device* Model::device_ = nullptr;
UINT Model::descriptorHandleIncrementSize_ = 0;

void Model::StaticInitialize(ID3D12Device* device)
{
	// nullptrチェック
	assert(device);

	Model::device_ = device;

	// メッシュの静的初期化
	Mesh::StaticInitialize(device);
}

std::unique_ptr<Model> Model::CreateFromObject(const std::string& text, bool smoothing)
{
	// 3Dオブジェクトのインスタンスを生成
	Model* model = new Model();
	if (model == nullptr)
	{
		return nullptr;
	}

	// 初期化
	model->Initialize(text, smoothing);
	if (model == nullptr)
	{
		delete model;
		assert(0);
		return nullptr;
	}

	return std::unique_ptr<Model>(model);
}

void Model::Initialize(const std::string& text, bool smoothing)
{
	//ファイルストリーム
	std::ifstream file;
	//.odjファイルを開く
	//file.open("Resources/triangle/triangle_tex.obj");

	const string modelname = text;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	name_ = modelname;

	// メッシュ生成
	Mesh* mesh = new Mesh;
	int indexCountTex = 0;
	int indexCountNoTex = 0;

	vector<XMFLOAT3> positions;  //頂点座標
	vector<XMFLOAT3> normals;    //法線ベクトル
	vector<XMFLOAT2> texcoords;  //テクスチャUV
	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列がmtllibなら頂点座標
		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		// 先頭文字列がgならグループの開始
		if (key == "g")
		{
			// カレントメッシュの情報が揃っているなら
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0)
			{
				//頂点法線の平均によるエッジの平滑化
				if (smoothing)
				{
					mesh->CalculateSmoothedVetexNormals();
				}
				// コンテナに登録
				meshes_.emplace_back(mesh);
				// 次のメッシュ生成
				mesh = new Mesh;
				indexCountTex = 0;
			}

			// グループ名読み込み
			string groupName;
			line_stream >> groupName;

			// メッシュに名前をセット
			mesh->SetName(groupName);
		}

		//先頭文字列がvなら頂点座標
		if (key == "v")
		{
			//X.Y.Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
			//頂点データに追加
			//VertexPosNormalUv vertex{};
			//vertex.pos = position;
			//vertices.emplace_back(vertex);
		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			//U,V座標読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		//先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			//X.Y.Z座標読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		// 先頭文字列がusemtlならマテリアルを割り当てる
		if (key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr)
			{
				// マテリアルの名読み込み
				string materialName;
				line_stream >> materialName;

				// マテリアル名で検索し、マテリアルを割り当てる
				auto itr = materials_.find(materialName);
				if (itr != materials_.end())
				{
					mesh->SetMaterial(itr->second.get());
				}
			}
		}

		//先頭文字列がfならポリゴン(三角形)
		if (key == "f")
		{
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			int count = 0;

			while (getline(line_stream, index_string, ' '))
			{
				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexTexcoord, indexNormal;
				// 頂点番号
				index_stream >> indexPosition;

				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				// マテリアル、テクスチャがある場合
				if (material && material->textureFilename_.size() > 0)
				{
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
					index_stream >> indexNormal;
					// 頂点データの追加
					Mesh::VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					mesh->AddVertex(vertex);
				}
				else
				{
					char c;
					index_stream >> c;
					// スラッシュ2連続の場合、頂点番号のみ
					if (c == '/')
					{
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}
					else
					{
						index_stream.seekg(-1, ios_base::cur); // 1文字戻る
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
						index_stream >> indexNormal;
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);

						//エッジ平滑化用のデータを追加
						if (smoothing)
						{
							//vキー(座標データ)の番号と、全て合成した頂点のインデックスをセットで登録する
							mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
						}
					}
				}

				// インデックスデータの追加
				if (count > 2)
				{
					// 四角形ポリゴンの4点目なので、
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築する
					mesh->AddIndex(indexCountTex - 1);
					mesh->AddIndex(indexCountTex);
					mesh->AddIndex(indexCountTex - 3);
				}
				else
				{
					mesh->AddIndex(indexCountTex);
				}
				indexCountTex++;
				count++;
			}
		}
	}
	//ファイルを閉じる
	file.close();

	//頂点法線の平均によるエッジの平滑化
	if (smoothing)
	{
		mesh->CalculateSmoothedVetexNormals();
	}

	// コンテナに登録
	meshes_.emplace_back(mesh);

	// メッシュのマテリアルチェック
	for (auto& m : meshes_)
	{
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr)
		{
			if (defaultMaterial_ == nullptr)
			{
				// デフォルトマテリアルを生成
				defaultMaterial_ = Material::Create();
				defaultMaterial_->name_ = "no material";
				materials_.emplace(defaultMaterial_->name_, defaultMaterial_);
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial_);
		}
	}

	// メッシュのバッファ生成
	for (auto& m : meshes_)
	{
		m->CreateBuffers();
	}

	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials_)
	{
		m.second->Update();
	}

	// デスクリプタヒープ生成
	CreateDescriptorHeap();

	// テクスチャの読み込み
	LoadTexture();
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	Material* material = nullptr;

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin()); //先頭の文字を削除
		}

		//先頭の文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{//マテリアル名読み込み
			// 既にマテリアルがあれば
			if (material)
			{
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create();
			// マテリアル名読み込み
			line_stream >> material->name_;
		}

		//先頭の文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material->ambient_.x;
			line_stream >> material->ambient_.y;
			line_stream >> material->ambient_.z;
		}

		//先頭の文字列がKdならディフューズ色
		if (key == "Kd")
		{
			line_stream >> material->diffuse_.x;
			line_stream >> material->diffuse_.y;
			line_stream >> material->diffuse_.z;
		}

		//先頭の文字列がKsならスペキュラー色
		if (key == "Ks")
		{
			line_stream >> material->specular_.x;
			line_stream >> material->specular_.y;
			line_stream >> material->specular_.z;
		}

		//先頭の文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			// テクスチャのファイル名読み込み
			line_stream >> material->textureFilename_;

			// フルパスからファイル名を取り出す
			size_t pos1;
			pos1 = material->textureFilename_.rfind('\\');
			if (pos1 != string::npos)
			{
				material->textureFilename_ = material->textureFilename_.substr(pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}

			pos1 = material->textureFilename_.rfind('/');
			if (pos1 != string::npos)
			{
				material->textureFilename_ = material->textureFilename_.substr(pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}
		}
	}
	//ファイルを閉じる
	file.close();

	if (material)
	{
		// マテリアルを登録
		AddMaterial(material);
	}
}

void Model::AddMaterial(Material* material)
{
	// コンテナに登録
	materials_.emplace(material->name_, material);
}

void Model::CreateDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// マテリアルの数
	size_t count = materials_.size();

	// デスクリプタヒープを生成	
	if (count > 0)
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
		descHeapDesc.NumDescriptors = (UINT)count; // シェーダーリソースビューの数
		result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap_));//生成

		if (FAILED(result))
		{
			assert(0);
		}
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Model::LoadTexture()
{
	int textureIndex = 0;
	string directoryPath = "Resources/" + name_ + "/";

	for (auto& m : materials_)
	{
		Material* material = m.second.get();

		// テクスチャあり
		if (material->textureFilename_.size() > 0)
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap_->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap_->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);
			// マテリアルにテクスチャ読み込み
			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);

			textureIndex++;
		}
		// テクスチャなし
		else
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap_->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap_->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);
			// マテリアルにテクスチャ読み込み
			material->LoadTexture("Resources/", cpuDescHandleSRV, gpuDescHandleSRV);

			textureIndex++;
		}
	}
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// nullptrチェック
	assert(device_);

	// デスクリプタヒープの配列
	if (descHeap_)
	{
		ID3D12DescriptorHeap* ppHeaps[] = { descHeap_.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	}

	descHeap_->SetName(L"Model");

	// 全メッシュを描画
	for (auto& mesh : meshes_)
	{
		mesh->Draw(cmdList);
	}
}

void Model::SetAlpha(float alpha)
{
	for (auto& m : materials_)
	{
		m.second->alpha_ = alpha;
		m.second->Update();
	}
}