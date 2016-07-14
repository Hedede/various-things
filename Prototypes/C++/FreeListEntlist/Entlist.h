struct EntManager {
	Entity* GetEntity(EntId id);
	void DeleteEntity(EntId id);

private:
	friend Entity::Entity();
	EntId InsertEntity(Entity& ent);

	std::vector<EntId> freelist;
	std::vector<Entity*> entlist;
};

struct EntId {
	u32 index;
	u32 version;
};

EntId EntManager::InsertEntity(Entity& ent)
{
	if (freelist.empty()) {
		EntId id {ents.size() - 1, 0};
		ent.setId(id);
		ents.emplace_back(&ent);
		return id;
	}

	auto id = freelist.back();
	freelist.pop_back();

	++id.version;

	ents[id] = &ent;

	return id;
}

Entity* EntManager::GetEntity(EntId id)
{
	if (id.index > ents.size())
		return nullptr;
	auto ent = ents[id.index];
	if (!ent || ent->id.version != id.version)
		return nullptr;
	return ent;
}

void* EntManager::DeleteEntity(EntId id)
{
	auto ent = GetEntity(id);
	if (ent)
		return;
	freelist.push_back(ent->id);
	delete ent;
}
