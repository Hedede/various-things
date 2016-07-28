	// .h
	namespace wrapper {
	class Node {
	public:
		virtual setParent(Node* newParent) = 0;
		virtual addChild(Node* newChild) = 0;
	}

	class NodeA : Node {
	public:
		virtual setParent(Node* newParent) = 0;
		virtual addChild(Node* newChild) = 0;
	}

	class NodeB : Node {
	public:
		virtual setParent(Node* newParent) = 0;
		virtual addChild(Node* newChild) = 0;
	}

	// .cpp
	namespace wrapper {
	class implNodeA : NodeA {
	public:
		virtual setParent(Node* newParent)
		{
			// fuck, I don't know if its impl::NodeA
			// or impl::NodeB, how do I do this?
		}
		virtual addChild(Node* newChild);

		other_library::iNode getUnder()
		{
			return nodeA_vnizu;
		}
	private:
		other_library::cNodeA* nodeA_vnizu;
	}

	class implNodeB : NodeB {
	public:
		virtual setParent(Node* newParent);
		virtual addChild(Node* newChild);
		other_library::iNode getUnder()
		{
			return nodeB_vnizu;
		}
	private:
		other_library::cNodeB* nodeB_vnizu;
	}
	}


// fuck, how I implement without getNodeVnizu() in public intorfaes
