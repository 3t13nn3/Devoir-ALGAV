#include "head.h"

/*
arg: 0 Initial Tree, 1 Tree to add
Insert a Tree in the Parents childrens
*/
Node* insert(Node* initTree, Node* treeToAdd) {

    if(initTree->_leftChild == nullptr){
	    initTree->_leftChild = treeToAdd;
    } else {
        initTree->_rightChild = treeToAdd;
    }

	return treeToAdd;
}

/*
arg: 0 width of the tree (length of the table of truth generaly)
return the height of the tree
*/
int getTreeHeightFromWidth(int width){
	int power = 1;

	//power is the size of our tree (pow of 2)
	while(1<<power < width) {
		power++;
	}

	return power;
}

/*
arg: 0 Initial Tree, 1 Height of the tree, 2 table of truth
Create a Tree of a Height "height", based on DFS, putting the table of truth value at leafs
*/
void createTreeFromTable(Node* tree, int height, std::string & table) {

	if (height == 0) {
		tree->_value = table[0];

        //removing the first element from table
		table = {table.begin() + 1, table.end()};
		return;
	}

	insert(tree, new Node);
	createTreeFromTable(tree->_leftChild, height-1, table);
    
	insert(tree, new Node);
	createTreeFromTable(tree->_rightChild, height-1, table);
}

/*
arg: 0 table of truth
Create a Tree of a certain height in function of the table of truth
*/
Node* consArbre(std::string table) {
	Node* t = new Node;

	createTreeFromTable(t, getTreeHeightFromWidth(table.size()), table);

	return t;
}

/*
arg: 0 Root node
Free all the children of a given node
*/
void freeAllChildren(Node* n){

	if(n->_leftChild != nullptr){
		freeAllChildren(n->_leftChild);
	}
	
	if(n->_rightChild != nullptr){
		freeAllChildren(n->_rightChild);
	}

	delete n;
}

/*
arg: 0 stage of the node (call it with 0, a root is alaways as 0), 1 the root
Print all the children of a given node
*/
void printAllChildren(int stage, Node* n){

	if(n->_leftChild != nullptr){
		printAllChildren(stage+1, n->_leftChild);
	}
	
	if(n->_rightChild != nullptr){
		printAllChildren(stage+1, n->_rightChild);
	}
	int i;
	for(i=0; i< stage; i++){
		std::cout << "- - - -\t\t";
        
	}
	std::cout << "[" << stage << "]" << "Node: ";
    std::cout << n->_value << std::endl;
}

void luka(Node* n) {

    if (n->_leftChild == nullptr) {
		return;
	}

    luka(n->_leftChild);
    
	luka(n->_rightChild);

    n->_value = n->_leftChild->_value + n->_rightChild->_value;
}

void defineInDot(Node* n, int height, std::ofstream & f) {

    if (n  == nullptr) {
		return;
	}

    const void * address = static_cast<const void*>(n);
    std::stringstream ss;
    ss << address;  
    std::string name = ss.str(); 
    name = "_" + name + "_";

    std::string toWrite = name + "[label=\"x" + std::to_string(height) + " [" + n->_value + "]\"];";
    f << toWrite;
    defineInDot(n->_leftChild, height-1, f);

    defineInDot(n->_rightChild, height-1, f);

    
}

void linkInDot(Node* n, std::ofstream & f) {

    if (n->_leftChild == nullptr && n->_rightChild == nullptr) {
		return;
	}

    const void * address = static_cast<const void*>(n);
    std::stringstream ss;
    ss << address;  
    std::string father = ss.str(); 
    father = "_" + father + "_";

    if (n->_leftChild != nullptr) {
        
        const void * address = static_cast<const void*>(n->_leftChild);
        std::stringstream ss;
        ss << address;  
        std::string child = ss.str(); 
        child = "_" + child + "_";

        std::string toWrite = father + "->" + child+ "[style=dashed];";
        f << toWrite;

    }

    if (n->_rightChild != nullptr) {
        
        const void * address = static_cast<const void*>(n->_rightChild);
        std::stringstream ss;
        ss << address;  
        std::string child = ss.str(); 
        child = "_" + child + "_";

        std::string toWrite = father + "->" + child+ ";";
        f << toWrite;

    }

    linkInDot(n->_leftChild, f);

    linkInDot(n->_rightChild, f);

    
}

void dot(Node* t) {
	//exemple: digraph{a->bb[style=dashed];b[label=\"som1\"];b->c[labal="som1"];a->c;d->c;e->c;e->a;}

    system("mkdir ../tree");
    std::ofstream dotFile ("../tree/tree.dot");

    if (dotFile.is_open()){

        dotFile << "digraph{";

        int height = getTreeHeightFromWidth(t->_value.size());

        defineInDot(t, height, dotFile);

        linkInDot(t, dotFile);

        dotFile << "}";

        dotFile.close();
    }

    std::cout << "using dot -Tsvg -o tree/tree.svg tree/tree.dot" << std::endl;

	//creating tree image
	system("dot -Tsvg -o ../tree/tree.svg ../tree/tree.dot");
    
    /*


	indexs = append([]int{0}, indexs...)
	fmt.Println(len(visited))
	height = len(indexs) - 1

	for height > 0 {

		childStart := indexs[len(indexs)-1]
		fatherStart := indexs[len(indexs)-2]
		father := visited[fatherStart:childStart]

		for _, fa := range father {

			endStr := ""

			for i, c := range fa.children {

				if i%2 == 0 {
					endStr = "[style=dashed];"
				} else {
					endStr = ";"
				}

				_, err = f.WriteString(fmt.Sprintf("%p", fa)[2:] + "->" + fmt.Sprintf("%p", c)[2:] + endStr)
				if err != nil {
					log.Fatal(err)
				}
			}

		}

		height--
		visited = visited[:childStart]
	}

	//end with
	_, err = f.WriteString("}")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("using dot -Tsvg -o tree/tree.svg tree/tree.dot")
	//creating tree image
	exec.Command("dot -Tsvg -o tree/tree.svg tree/tree.dot").Output()*/
}