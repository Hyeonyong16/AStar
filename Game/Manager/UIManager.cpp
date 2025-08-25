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
	// asset �� �Ҵ� ����
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

	

	// �տ� �ѵ� �̹��� ������
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

// ���� �̸����� ������ �ҷ��ͼ� �����صδ� �Լ�
void UIManager::AssetLoading(const char* _fileName)
{
	// ���� ���� ��� �ϼ�
	char filePath[256] = { };
	sprintf_s(filePath, 256, "../Assets/%s", _fileName);

	FILE* file = nullptr;
	fopen_s(&file, filePath, "rt");

	// ����ó��
	if (nullptr == file)
	{
		std::cout << "�� ���� �б� ����." << _fileName << "\n";
		__debugbreak();
		return;
	}
	int offset = 0;
	int assetWidth = 0;
	int assetHeight = 0;
	char buffer[50] = { };

	// ���� ������ ���ؼ� key, width, Height ���� �ҷ���
	fscanf_s(file, "key=%s\n", buffer, 50);
	fscanf_s(file, "width=%d\n", &assetWidth);
	fscanf_s(file, "height=%d\n", &assetHeight);
	offset = ftell(file);


	// �Ľ�(Parcing, �ؼ�)
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file) - offset;
	fseek(file, offset, SEEK_SET);

	// Ȯ���� ���� ũ�⸦ Ȱ���� ���� �Ҵ�
	char* image = new char[fileSize + 1];
	memset(image, 0, fileSize + 1);
	size_t readSize = fread(image, sizeof(char), fileSize, file);

	// �迭 ��ȸ�� ���� �ε��� ����
	int index = 0;

	// ���ڿ� ���� ��
	int size = (int)readSize;

	//���� �迭 ��ȸ
	while (index < size)
	{
		// �� ���� Ȯ��
		char mapCharacter = image[index++];
	}
	Asset* asset = new Asset(buffer, assetWidth, assetHeight, image);
	assets.emplace_back(asset);

	// ���� ����
	delete[] image;

	// ���� �ݱ�
	fclose(file);
}
