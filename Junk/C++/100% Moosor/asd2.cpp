	// .h
	namespace wrapper {
	class Node {
	public:
		virtual setParent(Node* newParent) = 0;
		virtual impl::Node* getImpl()
		{
			return impl;
		}
	private:
		impl::Node impl*;
	}

	class NodeA : Node {
	public:
		virtual setParent(Node* newParent)
		{
			impl->setParent(newParent->getImpl);
		}
		virtual impl::Node* getImpl()
		{
			return impl;
		}

	private:
		impl::NodeA* impl;
	}

	// .cpp
	namespace wrapper {
	namespace impl {
	class Node {
	public:
		virtual setParent(Node* node) = 0;		
		virtual other_library::iNode* getNode() = 0;
	}

	class NodeA : NodeA {
	public:
		virtual setParent(Node* node)
		{
			// motherfucking six-month break from motherfucking hrengin gave results
			node->getNode()->setParent(cNodeA_m);
		}

		virtual other_library::iNode* getNode()
		{
			return cNodeA_m;
		}
	private:
		other_library::cNodeA* cNodeA_m;
	}

	// NodeB analom gichno
	}
	}

