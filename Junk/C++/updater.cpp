
class Updater {
public:
	std::vector<NodeUpdate> data;
	
	void set_status(bool status) {
		m.lock();
		updated = status;
		m.unlock()
	}

	bool has_updates() {
		return updated;
	}
private:
	bool updated;
	std::mutex m;
}

int main() {
	Updater* u = new Updater();

	// ...
	
	while (run) {
		game->update();

		if (!u->has_updates()) {
			copy(game->getUpdate(), u->data);
			u->set_status(true);
		}
	}
}

int graphics(Updater* u) {
	while (run) {
		if(med->has_updates()) {
			copy(med->data, tmp);
			u->set_status(false);
		}
	}
}
