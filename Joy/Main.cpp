#include <Windows.h>

#include<d3d11.h>


struct  Backend
{
	ID3D11Device* dev;
};

class GameClass 
{
public:
	static GameClass& Get()
	{
		static GameClass instance;
		return instance;
	}


private:
	GameClass();

public:
	GameClass(const GameClass& other) = delete;
	GameClass operator=() = delete;

};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevHInstance, _In_ LPWSTR lpwstr, _In_ int showCmd)
{
	GameClass::Get().dev;

	return 0;
}