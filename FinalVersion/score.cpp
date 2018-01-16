//node(x,y) is [starting node] nodes of the board
//oldcolor is 0/1/2 of the current node
//newcolor is color of the area contour
//may need a function to find the node before scoring

int getOldColor(int x0,int y0){
	int x=x0;
	int y=y0;
	int color[3];
	for(int i;i<4;i++){
		color[i]=0;
	}
	while(x<=9){
		if(board[x][y]!=0){
			color[0]=board[x][y];
			break;
		}
		else x++;
	}
	x=x0;
	while(x>=0){
		if(board[x][y]!=0){
			color[1]=board[x][y];
			break;
		}
		else x--;
	}
	x=x0;
	while(y<=9){
		if(board[x][y]!=0){
			color[2]=board[x][y];
			break;
		}
		else y++;
	}
	y=y0;
	while(y>=0){
		if(board[x][y]!=0){
			color[3]=board[x][y];
			break;
		}
		else y--;
	}
	//if there's a match ie. 1 black 2 white 3 border(then detect more)
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
		if(color[i]==color[j] && color[i]!=3) return color[i];
		else if(color[i]==3 && color[i]==color[j]) return 1;
		}

	}
}

int score(int x,int y,int oldcolor,int newcolor){
	//find the node(empty spots)indirectly record the position of
	//empty spots to be used as starting nodes
		if(node[x][y]==0 || node[x][y]==oldcolor)
		{
			score(x+1,y,oldcolor,newcolor);
			score(x,y+1,oldcolor,newcolor);
			score(x-1,y,oldcolor,newcolor);
			score(x,y-1,oldcolor,newcolor);
		}
}

int main(void){
	int node[9][9];//can be created by Point2f
	int blackScore=0;
	int whiteScore=0;
	int oldcolor;

	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			if(i==0||i==9||j==0||j==9) board[i][j]=3;
			//set border value to 3 
		}
	}

	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
		node[i][j]=-1;
		}
	}

	//find the node(empty spots)indirectly record the position of
	//empty spots to be used as starting nodes
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
		if(board[i][j]==0) node[i][j]=0;//"board" is the stones on
		//the current board
		}
	}
	//get oldcolor
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			if(node[i][j]==0) oldcolor=getOldColor(i,j);
		}
	}

	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
		  if(node[i][j]==1) blackScore = score(i,j,oldcolor,1);
		}
	}

	whiteScore = 81-blackScore;
}
