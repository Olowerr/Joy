#include <Windows.h>

#include<d3d11.h>


struct  Backend
{
	ID3D11Device* dev;
};

class GameClass 
{
public:
	static Backend& Get()
	{
		static GameClass instance;
		return instance.system;
	}


private:
	GameClass();
	Backend system;

public:
	GameClass(const GameClass& other) = delete;
	GameClass& operator=(const GameClass& other) = delete;
	GameClass(GameClass&& other) = delete;

};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevHInstance, _In_ LPWSTR lpwstr, _In_ int showCmd)
{
	GameClass duegd;

	return 0;
}