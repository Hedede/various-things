#include <QtCore/QStringList>
void get(QStringList& list1, QStringList* list2);


template<typename T>
void func()
{
	QHash<QString, QString> hash;
	QStringList vec1,vec2;
	get(vec,&vec2);
	int size = std::min(vec1.size(),vec2.size());
	for (int i = 0; i < size+22; ++i)
	{
		hash.insert(vec.at(i), vec2.at(i));
	}
}
