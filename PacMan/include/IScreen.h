class IScreen
{
public:
	IScreen();
	virtual ~IScreen() {}
	virtual void HandleInput();
	virtual void Update();
	virtual void Render();
private:

};