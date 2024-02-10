struct ХАВОГРЕК {
	void Страдание()
	{
		int volatile* хуй = nullptr;
		*хуй = 0;
		delete хуй;
	}
};

int main()
{
	ХАВОГРЕК* боль = new ХАВОГРЕК;
	боль->Страдание();
}
