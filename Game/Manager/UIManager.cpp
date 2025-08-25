#include "UIManager.h"
#include "Engine.h"
#include "Level/GameLevel.h"

#include "Game/Game.h"

#include <iostream>

UIManager::UIManager(GameLevel* _gameLevel)
	: Actor(), gameLevel(_gameLevel)
{
	AssetLoading("CharacterFace.txt");
	AssetLoading("GunImage.txt");
	AssetLoading("HandWithGunImage.txt");
	AssetLoading("Num1.txt");
	AssetLoading("Num2.txt");
}

UIManager::~UIManager()
{
	// asset 들 할당 해제
	for (Asset* asset : assets)
	{
		SafeDelete(asset);
	}

	assets.clear();
}

void UIManager::BeginPlay()
{
	super::BeginPlay();
}

void UIManager::Tick(float _deltaTime)
{

}

void UIManager::Render()
{
	int consoleWidth = Engine::Get().GetWidth();
	int consoleHeight = Engine::Get().GetHeight();

	

	// 손에 총든 이미지 렌더링
	/*{
		Asset* characterAsset = nullptr;
		for (Asset* asset : assets)
		{
			if (strcmp(asset->GetKey(), "HandWithGunImage") == 0)
			{
				characterAsset = asset;
				break;
			}
		}

		if (nullptr != characterAsset)
		{
			int xPos = gameCamera->GetScreenWidth() / 2 - (characterAsset->GetWidth() / 2) + 1;
			int yPos = gameCamera->GetScreenHeight() + 1 - characterAsset->GetHeight();

			int imageSize = characterAsset->GetHeight() * characterAsset->GetWidth();
			for (int i = 0; i < imageSize; ++i)
			{
				if (characterAsset->GetImage()[i] == '\n' || characterAsset->GetImage()[i] == '\0'
					|| characterAsset->GetImage()[i] == ':')
				{
					continue;
				}

				Color tempColor = Color::White;

				if (characterAsset->GetImage()[i] == 'o')
				{
					tempColor = Color::YellowIntensity;
				}

				Engine::Get().WriteToBuffer(
					Vector2((i % (characterAsset->GetWidth())) + xPos, (i / characterAsset->GetWidth()) + yPos),
					characterAsset->GetImage()[i],
					tempColor
				);
			}
		}

	}*/
}

// 파일 이름으로 에셋을 불러와서 저장해두는 함수
void UIManager::AssetLoading(const char* _fileName)
{
	// 최종 에셋 경로 완성
	char filePath[256] = { };
	sprintf_s(filePath, 256, "../Assets/%s", _fileName);

	FILE* file = nullptr;
	fopen_s(&file, filePath, "rt");

	// 예외처리
	if (nullptr == file)
	{
		std::cout << "맵 파일 읽기 실패." << _fileName << "\n";
		__debugbreak();
		return;
	}
	int offset = 0;
	int assetWidth = 0;
	int assetHeight = 0;
	char buffer[50] = { };

	// 에셋 저장을 위해서 key, width, Height 정보 불러옴
	fscanf_s(file, "key=%s\n", buffer, 50);
	fscanf_s(file, "width=%d\n", &assetWidth);
	fscanf_s(file, "height=%d\n", &assetHeight);
	offset = ftell(file);


	// 파싱(Parcing, 해석)
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file) - offset;
	fseek(file, offset, SEEK_SET);

	// 확인할 파일 크기를 활용해 버퍼 할당
	char* image = new char[fileSize + 1];
	memset(image, 0, fileSize + 1);
	size_t readSize = fread(image, sizeof(char), fileSize, file);

	// 배열 순회를 위한 인덱스 변수
	int index = 0;

	// 문자열 길이 값
	int size = (int)readSize;

	//문자 배열 순회
	while (index < size)
	{
		// 맵 문자 확인
		char mapCharacter = image[index++];
	}
	Asset* asset = new Asset(buffer, assetWidth, assetHeight, image);
	assets.emplace_back(asset);

	// 버퍼 해제
	delete[] image;

	// 파일 닫기
	fclose(file);
}
