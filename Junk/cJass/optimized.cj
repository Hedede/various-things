void ApplyRecipe() {
	unit u = GetTriggerUnit();
	int i = 0, j = 0, k = 0;
	item indexItem, item1, item2, item3, item4;
	do {
		item1 = null;
		item2 = null;
		item3 = null;
		item4 = null;
		count = i_recipe_length[i];
		
		do {
			indexItem = UnitItemInSlot(u, j)
			if(indexItem != null) {
				if (item1 == null) and (GetItemTypeId(indexItem) == i_recipe_item1[i]) {
					item1 = indexItem;
					k ++;
				} else if (item2 == null) and (GetItemTypeId(indexItem) == i_recipe_item2[i]) {
					item2 = indexItem;
					k ++;
				} else if (item3 == null) and (GetItemTypeId(indexItem) == i_recipe_item3[i]) {
					item3 = indexItem;
					k ++;
				} els eif (item4 == null) and (GetItemTypeId(indexItem) == i_recipe_item4[i]) {
					item4 = indexItem;
					k ++;
				}
			}
		} while (j++ <= bj_MAX_INVENTORY);
		
		if(k == count) {
			RemoveItem(item1);
			if(count > 1) {
				RemoveItem(item2);
			}
			if(count > 2) {
				RemoveItem(item3);
			}
			if(count > 3) {
				RemoveItem(item4);
			}
			DestroyEffect(AddSpecialEffectTarget("Abilities\\Spells\\Items\\AIem\\AIemTarget.mdl", u, "origin"));
			UnitAddItemById(u,i_result_item[i]);
			return;
		}
	} while (i++ < i_max_recipes);
}

void StartTrigger_Try_Apply_Recipe () {
	gg_trg_Try_Apply_Recipe = new trigger();
	TriggerRegisterAnyUnitEventBJ(gg_trg_Try_Apply_Recipe,EVENT_PLAYER_UNIT_PICKUP_ITEM);
	TriggerAddAction(gg_trg_Try_Apply_Recipe,Trig_Try_Apply_Recipe_Actions);
}
