#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <string>

using namespace std;

int fileN = 1000;
string folder;

class Node
{
	int t;

	void read()
	{
		std::ifstream input{ folder + "/" + fileName, std::ifstream::binary };
		std::vector<int> vector;

		// Читаем размер вектора
		int size = 0;
		input.read(reinterpret_cast<char*>(&size), sizeof(size));

		// Читаем элементы вектора
		int item;
		for (int i = 0; i < size; ++i) {
			input.read(reinterpret_cast<char*>(&item), sizeof(int));
			vector.push_back(item);
		}
		keys = vector;
		cntKeys = size;

		// Читаем размер вектора
		int size3 = 0;
		input.read(reinterpret_cast<char*>(&size3), sizeof(size3));

		// Читаем элементы вектора
		string itemStr;
		for (int i = 0; i < size3; ++i) {
			input.read(reinterpret_cast<char*>(&size), sizeof(int));
			char* str = new char[size + 1];
			input.read(str, size);
			str[size] = 0;
			itemStr = static_cast<string>(str);
			values.emplace_back(itemStr);
			delete[] str;
		}

		input.read(reinterpret_cast<char*>(&isLeaf), sizeof(bool));

		// Читаем размер вектора
		int size2 = 0;
		input.read(reinterpret_cast<char*>(&size2), sizeof(size2));

		// Читаем элементы вектора строк.
		string child;
		for (int i = 0; i < size2; ++i) {
			input.read(reinterpret_cast<char*>(&size), sizeof(int));
			char* str = new char[size + 1];
			input.read(str, size);
			str[size] = 0;
			child = static_cast<string>(str);
			children.emplace_back(child);
			delete[] str;
		}
		input.close();
	}
public:
	int cntKeys;
	vector<int> keys;
	vector<string> values;
	bool isLeaf;
	vector<string> children;
	string fileName;

	Node() = default;

	Node(int t_)
	{
		fileName = std::to_string(fileN++);
		t = t_;
	}

	Node(string filename, int t_)
	{
		fileName = filename;
		t = t_;
		read();
	}

	Node(int cnt, vector<int> keys, bool isLeaf, vector<string> children, vector<string> values, string filename, int t_)
	{
		cntKeys = cnt;
		this->keys = keys;
		this->isLeaf = isLeaf;
		this->children = children;
		this->values = values;
		fileName = filename;
		t = t_;
	}

	void write()
	{
		std::ofstream output{ folder + "/" + fileName, std::ofstream::out | std::ofstream::binary };
		// Записываем размер вектора
		int size = cntKeys;
		output.write(reinterpret_cast<const char*>(&size), sizeof(int));

		int i = 0;
		// Записываем элементы вектора
		for (auto& item : keys) {
			if (i++ >= cntKeys) break;
			output.write(reinterpret_cast<const char*>(&item), sizeof(int));
		}

		output.write(reinterpret_cast<const char*>(&size), sizeof(int));

		i = 0;
		// Записываем элементы вектора
		for (auto& item : values) {
			if (i++ >= cntKeys) break;
			int s = item.size();
			output.write(reinterpret_cast<const char*>(&s), sizeof(int));
			output.write(item.c_str(), s);
		}

		// Записываем isLeaf.
		output.write(reinterpret_cast<const char*>(&isLeaf), sizeof(bool));

		// Записываем размер вектора
		int size2 = cntKeys + 1;
		output.write(reinterpret_cast<const char*>(&size2), sizeof(int));

		i = 0;
		// Записываем элементы вектора
		for (auto& item : children) {
			if (i++ > cntKeys) break;
			int s = item.size();
			output.write(reinterpret_cast<const char*>(&s), sizeof(int));
			//output << item;
			output.write(item.c_str(), s);
		}
		//delete[] str;
		output.close();
	}

	void toString()
	{
		int i;
		for (i = 0; i < cntKeys; i++)
		{
			// If this is not leaf, then before printing key[i], 
			// traverse the subtree rooted with child C[i]. 
			if (!isLeaf)
				Node(children[i], t).toString();
			cout << " " << keys[i];
		}

		// Print the subtree rooted with last child 
		if (!isLeaf)
			Node(children[i], t).toString();
	}

	string find(int key)
	{
		int i = 0;
		while (i < cntKeys && key > keys[i])
		{
			i++;
		}
		if (i < cntKeys && key == keys[i])
		{
			return values[i];
		}
		if (isLeaf)
		{
			return "";
		}
		Node child = Node(children[i], t);
		return child.find(key);
	}

	void splitChild(int i)
	{
		Node y = Node(children[i], t);
		Node z = Node(t);
		children.resize(cntKeys + 2);
		keys.resize(cntKeys + 1);
		values.resize(cntKeys + 1);
		y.children.resize(y.cntKeys + 2);
		y.keys.resize(y.cntKeys + 1);
		y.values.resize(y.cntKeys + 1);
		z.cntKeys = t - 1;
		z.children.resize(z.cntKeys + 2);
		z.keys.resize(z.cntKeys + 1);
		z.values.resize(z.cntKeys + 1);
		z.isLeaf = y.isLeaf;
		for (int j = 0; j < t - 1; j++)
		{
			z.keys[j] = y.keys[j + t];
			z.values[j] = y.values[j + t];
		}
		if (!y.isLeaf)
		{
			for (int j = 0; j < t; j++)
			{
				z.children[j] = y.children[j + t];
			}
		}
		y.cntKeys = t - 1;
		for (int j = cntKeys; j >= i + 1; j--)
		{
			children[j + 1] = children[j];
		}
		children[i + 1] = z.fileName;
		for (int j = cntKeys - 1; j >= i; j--)
		{
			keys[j + 1] = keys[j];
			values[j + 1] = values[j];
		}
		keys[i] = y.keys[t - 1];
		values[i] = y.values[t - 1];
		cntKeys++;
		y.write();
		z.write();
		write();
	}

	bool insert(int key, const string& value)
	{
		int i = cntKeys - 1;
		keys.resize(cntKeys + 1);
		values.resize(cntKeys + 1);
		if (isLeaf)
		{
			while (i >= 0 && key < keys[i])
			{
				keys[i + 1] = keys[i];
				values[i + 1] = values[i];
				i--;
			}
			if (i >= 0 && keys[i] == key)
			{
				i++;
				while (i < cntKeys)
				{
					keys[i] = keys[i + 1];
					values[i] = values[i + 1];
					i++;
				}
				return false;
			}
			keys[i + 1] = key;
			values[i + 1] = value;
			cntKeys++;
			write();
			return true;
		}
		while (i >= 0 && key < keys[i])
		{
			i--;
		}
		if (i >= 0 && keys[i] == key)
		{
			return false;
		}
		i++;
		Node child = Node(children[i], t);
		if (child.cntKeys == 2 * t - 1)
		{
			splitChild(i);
			if (key == keys[i])
			{
				return false;
			}
			if (key > keys[i])
			{
				i++;
			}
		}
		child = Node(children[i], t);
		return child.insert(key, value);
	}

	string deleteKey(int key)
	{
		int i = findKey(key);
		if (i < cntKeys && keys[i] == key)
		{
			if (isLeaf)
			{
				return deleteFromLeaf(i);
			}
			return deleteFromNotLeaf(i);
		}
		if (isLeaf)
		{
			return "";
		}
		bool flag = i == cntKeys;
		if (Node(children[i], t).cntKeys < t)
		{
			fillChild(i);
		}
		if (flag && i > cntKeys)
		{
			return Node(children[i - 1], t).deleteKey(key);
		}
		return Node(children[i], t).deleteKey(key);
	}

	string deleteFromLeaf(int i)
	{
		string val = values[i];
		for (int j = i + 1; j < cntKeys; j++)
		{
			keys[j - 1] = keys[j];
			values[j - 1] = values[j];
		}
		cntKeys--;
		write();
		return val;
	}

	string deleteFromNotLeaf(int i)
	{
		int key = keys[i];
		string val = values[i];
		Node child = Node(children[i], t);
		Node nextChild = Node(children[i + 1], t);
		if (child.cntKeys >= t)
		{
			vector<string> pred = findPred(i);
			keys[i] = stoi(pred[0]);
			values[i] = pred[1];
			child.deleteKey(stoi(pred[0]));
		}
		else if (nextChild.cntKeys >= t)
		{
			vector<string> succ = findSucc(i);
			keys[i] = stoi(succ[0]);
			values[i] = succ[1];
			nextChild.deleteKey(stoi(succ[0]));
		}
		else
		{
			mergeChildren(i);
			child = Node(children[i], t);
			child.deleteKey(key);
		}
		write();
		return val;
	}

	void fillChild(int i)
	{
		if (i != 0 && Node(children[i - 1], t).cntKeys >= t)
		{
			borrowFromPrev(i);
		}
		else if (i != cntKeys && Node(children[i + 1], t).cntKeys >= t)
		{
			borrowFromNext(i);
		}
		else
		{
			if (i != cntKeys) mergeChildren(i);
			else mergeChildren(i - 1);
		}
	}

	vector<string> findPred(int i)
	{
		Node child = Node(children[i], t);
		while (!child.isLeaf)
		{
			child = Node(child.children[child.cntKeys], t);
		}
		vector<string> a = { to_string(child.keys[child.cntKeys - 1]), child.values[child.cntKeys - 1] };
		return a;
	}

	vector<string> findSucc(int i)
	{
		Node child = Node(children[i + 1], t);
		while (!child.isLeaf)
		{
			child = Node(child.children[0], t);
		}
		vector<string> a = { to_string(child.keys[0]), child.values[0] };
		return a;
	}

	void borrowFromPrev(int i)
	{
		Node child = Node(children[i], t);
		Node bigChild = Node(children[i - 1], t);
		child.keys.resize(child.cntKeys + 1);
		child.values.resize(child.cntKeys + 1);
		child.children.resize(child.cntKeys + 2);
		for (int j = child.cntKeys - 1; j >= 0; j--)
		{
			child.keys[j + 1] = child.keys[j];
			child.values[j + 1] = child.values[j];
		}
		if (!child.isLeaf)
		{
			for (int j = child.cntKeys; j >= 0; j--)
			{
				child.children[j + 1] = child.children[j];
			}
			child.children[0] = bigChild.children[bigChild.cntKeys];
		}
		child.keys[0] = keys[i - 1];
		child.values[0] = values[i - 1];
		keys[i - 1] = bigChild.keys[bigChild.cntKeys - 1];
		values[i - 1] = bigChild.values[bigChild.cntKeys - 1];
		child.cntKeys++;
		bigChild.cntKeys--;
		child.write();
		bigChild.write();
		write();
	}

	void borrowFromNext(int i)
	{
		Node child = Node(children[i], t);
		Node bigChild = Node(children[i + 1], t);
		child.keys.resize(child.cntKeys + 1);
		child.values.resize(child.cntKeys + 1);
		child.children.resize(child.cntKeys + 2);
		child.keys[child.cntKeys] = keys[i];
		child.values[child.cntKeys] = values[i];
		if (!bigChild.isLeaf)
		{
			child.children[child.cntKeys + 1] = bigChild.children[0];
		}
		keys[i] = bigChild.keys[0];
		values[i] = bigChild.values[0];
		for (int j = 1; j < bigChild.cntKeys; j++)
		{
			bigChild.keys[j - 1] = bigChild.keys[j];
			bigChild.values[j - 1] = bigChild.values[j];
		}
		if (!bigChild.isLeaf)
		{
			for (int j = 1; j <= bigChild.cntKeys; j++)
			{
				bigChild.children[j - 1] = bigChild.children[j];
			}
		}
		child.cntKeys++;
		bigChild.cntKeys--;
		child.write();
		bigChild.write();
		write();
	}

	void mergeChildren(int i)
	{
		Node child = Node(children[i], t);
		Node nextChild = Node(children[i + 1], t);
		child.keys.resize(2 * t - 1);
		child.values.resize(2 * t - 1);
		child.children.resize(2 * t);
		child.keys[t - 1] = keys[i];
		child.values[t - 1] = values[i];
		for (int j = 0; j < nextChild.cntKeys; j++)
		{
			child.keys[t + j] = nextChild.keys[j];
			child.values[t + j] = nextChild.values[j];
		}
		if (!nextChild.isLeaf)
		{
			for (int j = 0; j <= nextChild.cntKeys; j++)
			{
				child.children[t + j] = nextChild.children[j];
			}
		}
		for (int j = i + 1; j < cntKeys; j++)
		{
			keys[j - 1] = keys[j];
			values[j - 1] = values[j];
		}
		for (int j = i + 2; j <= cntKeys;j++)
		{
			children[j - 1] = children[j];
		}
		child.cntKeys += nextChild.cntKeys + 1;
		cntKeys--;
		child.write();
		write();
	}

	int findKey(int key)
	{
		int i = 0;
		while (i < cntKeys && keys[i] < key)
		{
			i++;
		}
		return i;
	}

	string operator[] (int *index)
	{
		int i;
		for (i = 0; i < cntKeys; i++)
		{
			if (!isLeaf)
			{
				string s;
				try {
					s = Node(children[i], t)[index];
				}
				catch (exception)
				{
					s = "none";
				}
				if (s != "none") return s;
			}
			if (*index == 0)
			{
				return values[i];
			}
			(*index)--;
		}

		if (!isLeaf)
		{
			string s;
			try {
				s = Node(children[i], t)[index];
			}
			catch (exception)
			{
				throw exception("Error!");
			}
			return s;
		}
		throw exception("Error");
	}
};

class BTree
{
	Node root;
	int t;
public:
	BTree() = default;

	BTree(Node root_, int t_)
	{
		this->root = std::move(root_);
		t = t_;
	}

	/// <summary>
	/// Получить значение по ключу.
	/// Возвращает пустую строку, если такого ключа не было.
	/// </summary>
	/// <param name="key">Ключ.</param>
	/// <returns>Искомое значение.</returns>
	string get(int key)
	{
		return root.find(key);
	}

	/// <summary>
	/// Вставка нового значения.
	/// Возвращает false, если такой ключ уже есть.
	/// </summary>
	/// <param name="key">Новый ключ</param>
	/// <param name="value">Новое значение</param>
	/// <returns>Можно ли вставить такой ключ.</returns>
	bool insert(int key, const string& value)
	{
		if (root.cntKeys == 2 * t - 1)
		{
			Node s = Node(t);
			s.isLeaf = false;
			s.cntKeys = 0;
			s.children.resize(1);
			s.children[0] = root.fileName;
			root.write();
			s.splitChild(0);
			//root.write();
			root = s;
			return s.insert(key, value);
		}
		return root.insert(key, value);
	}

	/// <summary>
	/// Удаляет значение по ключу.
	/// Возвращает удалённое значение
	/// или пустую строку, если значения не было.
	/// </summary>
	/// <param name="key">Удаляемый ключ</param>
	/// <returns>Удалённое значение</returns>
	string remove(int key)
	{
		if (root.cntKeys == 0 && root.isLeaf)
		{
			return "";
		}
		string val = root.deleteKey(key);
		if (root.cntKeys == 0 && !root.isLeaf)
		{
			root = Node(root.children[0], t);
		}
		return val;
	}

	string operator[] (int i)
	{
		if (root.cntKeys == 0 && root.isLeaf)
		{
			throw exception("Error!");
		}
		try
		{
			return root[&i];
		}
		catch (exception)
		{
			throw exception("Error!");
		}
	}

	void toString()
	{
		root.toString();
		cout << endl;
	}
};

BTree tree;
int isTreeCreated = false;

list<string> ReadFromFile(const string& path)
{
	list<string> commands;
	ifstream in;
	in.open(path);
	if (in.is_open()) {
		string line;
		while (getline(in, line)) {
			commands.push_back(line);
		}
	}
	in.close();
	return commands;
}

void WriteToFile(const string& path, const string& text)
{
	ofstream out;
	out.open(path);
	if (out.is_open()) {
		out << text << endl;
	}
	out.close();
}

int main(int argc, char* argv[])
{
	int t = stoi(argv[1]);
	if (t <= 1)
	{
		cout << "t must be >= 2" << endl;
		return 0;
	}
	folder = argv[2];
	
	const string inputPath = argv[3];
	const string outputPath = argv[4];
	list<string> commands = ReadFromFile(inputPath);
	string ans;

	int key;
	string value;
	size_t first, second;
	for (string command : commands)
	{
		if (command[0] == 'i')
		{
			first = command.find(' ');
			second = command.find(' ', first + 1);
			key = stoi(command.substr(first + 1, second - first - 1));
			value = command.substr(second + 1, command.length() - second - 1);
			if (isTreeCreated && tree.insert(key, value))
			{
				ans += "true\n";
			}
			else if (isTreeCreated)
			{
				ans += "false\n";
			}
			else
			{
				tree = BTree(Node(1, { key }, true, { "1", "2" }, { value }, "root", t), t);
				ans += "true\n";
				isTreeCreated = true;
			}
		}
		else if (command[0] == 'd')
		{
			first = command.find(' ');
			key = stoi(command.substr(first + 1, command.length() - first - 1));
			if (!isTreeCreated)
			{
				ans += "null\n";
			}
			else
			{
				string a = tree.remove(key);
				if (!a.empty())
				{
					ans += a + "\n";
				}
				else
				{
					ans += "null\n";
				}
			}
		}
		else if (command[0] == 'f')
		{
			first = command.find(' ');
			key = stoi(command.substr(first + 1, command.length() - first - 1));
			if (!isTreeCreated)
			{
				ans += "null\n";
			}
			else
			{
				string a = tree.get(key);
				if (!a.empty())
				{
					ans += a + "\n";
				}
				else
				{
					ans += "null\n";
				}
			}
		}
		else
		{
			first = command.find(' ');
			key = stoi(command.substr(first + 1, command.length() - first - 1));
			if (!isTreeCreated)
			{
				ans += "null\n";
			}
			else
			{
				string a;
				try
				{
					a = tree[key];
					ans += a + "\n";
				}
				catch(exception ex)
				{
					ans += "null\n";
				}
			}
		}
	}

	WriteToFile(outputPath, ans);
}