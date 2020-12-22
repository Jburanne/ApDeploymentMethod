#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <math.h>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;

struct Point{
	int x;
	int y;
	Point(int x1, int y1):x(x1),y(y1){}
};

struct Line{
	Point endpoint1;
	Point endpoint2;
	Line(Point p1, Point p2): endpoint1(p1), endpoint2(p2){}
};

//��ȡ�߶���Ϣ 
vector<Line> readLines(string filepath){
	vector<Line> lines;
	ifstream fin(filepath);
	cout<<filepath<<endl;
	string line;
	while(getline(fin,line)){
		//cout<<line<<endl;
		istringstream sin(line);
		vector<int> fields(4,0);
		string field;
		int i = 0;
		while(getline(sin,field,',')){
			fields[i++] = stoi(field);
		}
		Point p1(fields[0],fields[1]);
		Point p2(fields[2],fields[3]);
		Line curline(p1,p2);
		lines.push_back(curline);	
	}
	return lines;
}

vector<int> readResult(string filepath){
	vector<int> ans;
	ifstream fin(filepath);
	string line;
	while(getline(fin,line)){
		istringstream sin(line);
		string field;
		while(getline(sin,field,' ')){
			ans.push_back(stoi(field));
		}	
	}
	return ans;	
}
//��ȡ�߽�
vector<int> getBorder(vector<Line>& lines){
	int minX = INT_MAX, maxX = INT_MIN, minY = INT_MAX, maxY = INT_MIN;
	for(int i = 0;i < lines.size();++i){
		Line l = lines[i];
		int x1 = l.endpoint1.x;
		int y1 = l.endpoint1.y;
		int x2 = l.endpoint2.x;
		int y2 = l.endpoint2.y;
		if(min(x1,x2) < minX) minX = min(x1,x2);
		if(max(x1,x2) > maxX) maxX = max(x1,x2);
		if(min(y1,y2) < minY) minY = min(y1,y2);
		if(max(y1,y2) > maxY) maxY = max(y1,y2);	
	}
	vector<int> ans = {minX,maxX,minY,maxY};
	return ans;
} 



//�ж������߶��Ƿ��ཻ 
bool isCrossed(Line line1, Line line2){
	//�ο�https://www.cnblogs.com/wuwangchuxin0924/p/6218494.html 
	long line1_x1 = line1.endpoint1.x;
	long line1_x2 = line1.endpoint2.x;
	long line2_x1 = line2.endpoint1.x;
	long line2_x2 = line2.endpoint2.x;
	long line1_y1 = line1.endpoint1.y;
	long line1_y2 = line1.endpoint2.y;
	long line2_y1 = line2.endpoint1.y;
	long line2_y2 = line2.endpoint2.y;
	
	//x���ϲ��ཻ���ཻ������false 
	if(min(line1_x1,line1_x2) > max(line2_x1,line2_x2) || min(line2_x1,line2_x2) > max(line1_x1,line1_x2)){
		return false;
	}
	//y���ϲ��ཻ���ཻ������false 
	if(min(line1_y1,line1_y2) > max(line2_y1,line2_y2) || min(line2_y1,line2_y2) > max(line1_y1,line1_y2)){
		return false;
	}
	//����������������ж�
	long u,v,w,z;
	u = (line2_x1 - line1_x1)*(line1_y2 - line1_y1) - (line1_x2 - line1_x1)*(line2_y1 - line1_y1);
	//cout<<u<<endl;
	v = (line2_x2 - line1_x1)*(line1_y2 - line1_y1) - (line1_x2 - line1_x1)*(line2_y2 - line1_y1);
	//cout<<v<<endl;
	w = (line1_x1 - line2_x1)*(line2_y2 - line2_y1) - (line2_x2 - line2_x1)*(line1_y1 - line2_y1);
	//cout<<w<<endl;
	z = (line1_x2 - line2_x1)*(line2_y2 - line2_y1) - (line2_x2 - line2_x1)*(line1_y2 - line2_y1);
	//cout<<z<<endl;
	return (u<=0 && v>=0 || u>=0 && v<=0) && (w<=0 && z>=0 || w>=0 && z<=0);
} 

//�ж�����֮�������ϰ���
bool hasBarrier(Point p1, Point p2, vector<Line>& lines){
	Line curline(p1,p2);
	for(Line l:lines){
		if(isCrossed(curline,l)) return true;
	}
	return false;
} 

//�ж�����֮���ϰ�������� 
int barrierNum(Point p1, Point p2, vector<Line>& lines){
	Line curline(p1,p2);
	int cnt = 0;
	for(Line l:lines){
		if(isCrossed(curline,l)) cnt++;
	}
	return cnt;
}

//��ֱ�߼�����򣬰��������� 
bool vlinesCompare(Line& l1, Line& l2){
	return l1.endpoint1.x <= l2.endpoint1.x;
}

//ˮƽ�߼�����򣬰���������
bool hlinesCompare(Line& l1, Line& l2){
	return l1.endpoint1.y <= l2.endpoint1.y;
} 

//�ж�����֮���ϰ�������������ϰ������ܽ�������һ�� 
int barrierNum1(Point p1, Point p2, vector<Line>& lines){
	Line curline(p1,p2);
	vector<Line> h_lines;
	vector<Line> v_lines;
	for(Line l:lines){
		if(isCrossed(curline,l)){
			if(l.endpoint1.x == l.endpoint2.x){
				v_lines.push_back(l);
			} else if(l.endpoint1.y == l.endpoint2.y){
				h_lines.push_back(l); 
			}	
		}
	}
	//����
	sort(v_lines.begin(),v_lines.end(),vlinesCompare);
	sort(h_lines.begin(),h_lines.end(),hlinesCompare);
	int cnt = 0;
	if(v_lines.size() > 0){
		cnt++;
		int last = 0;
		for(int i = 1;i < v_lines.size();++i){
			if(v_lines[i].endpoint1.x - v_lines[last].endpoint1.x > 500){
				cnt++;
				last = i;
			}
		}
	}
	if(h_lines.size() > 0){
		cnt++;
		int last = 0;
		for(int i = 1;i < h_lines.size();++i){
			if(h_lines[i].endpoint1.y - h_lines[last].endpoint1.y > 500){
				cnt++;
				last = i;
			}
		}
	}
	return cnt;	
}

//�жϵ��Ƿ����߶���
bool isOnLine(Point p, vector<Line>& lines){
	for(Line l:lines){
		int x1 = l.endpoint1.x;
		int x2 = l.endpoint2.x;
		int y1 = l.endpoint1.y;
		int y2 = l.endpoint2.y;
		if(x1 == x2 && p.x == x1 && p.y >= min(y1,y2) && p.y <= max(y1,y2)){
			return true;
		} 
		if(y1 == y2 && p.y == y1 && p.x >= min(x1,x2) && p.x <= max(x1,x2)){
			return true;
		}
	}
	return false;
} 

//�жϵ��Ƿ����߶���Χ
bool isNearLine(Point p, vector<Line>& lines){
	for(Line l:lines){
		int x1 = l.endpoint1.x;
		int x2 = l.endpoint2.x;
		int y1 = l.endpoint1.y;
		int y2 = l.endpoint2.y;
		if(x1 == x2 && abs(p.x - x1) < 500 && p.y >= min(y1,y2) && p.y <= max(y1,y2)){
			return true;
		} 
		if(y1 == y2 && abs(p.y - y1) < 500 && p.x >= min(x1,x2) && p.x <= max(x1,x2)){
			return true;
		}
	}
	return false;
} 

bool myCompare(pair<int,int>& a, pair<int,int>& b){
	return a.first < b.first;
}


//����϶��������߶�
vector<Line> mergeLines(vector<Line>& lines){
	map<int, vector<pair<int,int>>> h_lines;
	map<int, vector<pair<int,int>>> v_lines;
	for(Line l:lines){
		int x1 = l.endpoint1.x;
		int x2 = l.endpoint2.x;
		int y1 = l.endpoint1.y;
		int y2 = l.endpoint2.y;
		if(x1 == x2){
			pair<int,int> p(min(y1,y2), max(y1,y2));
			v_lines[x1].push_back(p);
		}else if(y1 == y2){
			pair<int,int> p(min(x1,x2), max(x1,x2));
			h_lines[y1].push_back(p);
		}
	}
	vector<Line> new_lines;
	map<int, vector<pair<int,int>>>::iterator iter;
	//����ˮƽ�� 
	for(iter = h_lines.begin(); iter != h_lines.end(); ++iter){
		int y = iter->first;
		vector<pair<int,int>> elem = iter->second;
		sort(elem.begin(),elem.end(),myCompare);
		int begin_pos = 0;
		int i = 0;
		while(i < elem.size()){
			if(i < elem.size()-1 && elem[i+1].first - elem[i].second <= 2000 && elem[i+1].first - elem[i].second >= 0) i++;
			else{
				//check�Ƿ��ظ�
				int k = new_lines.size()-1;
				int flag = 0;
				while(k >= 0 && new_lines[k].endpoint1.y == y){
					if(new_lines[k].endpoint1.x == elem[begin_pos].first && new_lines[k].endpoint2.x == elem[i].second){
						flag = 1;
						break;
					}
					k--;
				}
				if(flag == 1){
					begin_pos = i+1;
					i = begin_pos;
					continue;
				}
				Point p1(elem[begin_pos].first, y);
				Point p2(elem[i].second, y);
				Line temp(p1,p2);
				new_lines.push_back(temp);
				begin_pos = i+1;
				i = begin_pos;
			}
		}
	}
	//����ֱ��
	for(iter = v_lines.begin(); iter != v_lines.end(); ++iter){
		int x = iter->first;
		vector<pair<int,int>> elem = iter->second;
		sort(elem.begin(),elem.end(),myCompare);
		int begin_pos = 0;
		int i = 0;
		while(i < elem.size()){
			if(i < elem.size()-1 && elem[i+1].first - elem[i].second <= 2000 && elem[i+1].first - elem[i].second >= 0) i++;
			else{
				//check�Ƿ��ظ�
				int k = new_lines.size()-1;
				int flag = 0;
				while(k >= 0 && new_lines[k].endpoint1.x == x){
					if(new_lines[k].endpoint1.y == elem[begin_pos].first && new_lines[k].endpoint2.y == elem[i].second){
						flag = 1;
						break;
					}
					k--;
				} 
				if(flag == 1){
					begin_pos = i+1;
					i = begin_pos;
					continue;
				}
				Point p1(x, elem[begin_pos].first);
				Point p2(x, elem[i].second);
				Line temp(p1,p2);
				new_lines.push_back(temp);
				begin_pos = i+1;
				i = begin_pos;
			}
		}
	}
	return new_lines; 
} 

//����ȱ��
vector<Line> closeLines(vector<Line>& lines){
	vector<Line> ans;
	for(Line l:lines){
		//vertical line 
		if(l.endpoint1.x == l.endpoint2.x){
			int xpos = l.endpoint1.x;
			int ypos1 = l.endpoint1.y;
			int ypos2 = l.endpoint2.y;
			int flag = 0;
			for(Line l1:lines){	
				int x1 = l1.endpoint1.x;
				int x2 = l1.endpoint2.x;
				if(x1 == x2) continue;
				int y1 = l1.endpoint1.y;
				if(xpos >= min(x1,x2) && xpos <= max(x1,x2)){
					if(min(ypos1,ypos2) >= y1 && min(ypos1,ypos2) - y1 <= 1000){
						Point p1(xpos,max(ypos1,ypos2));
						Point p2(xpos,y1);
						Line temp(p1,p2);
						ans.push_back(temp);
						flag = 1;
						break;
					} else if(max(ypos1,ypos2) <= y1 && y1 - max(ypos1,ypos2) <= 1000){
						Point p1(xpos,min(ypos1,ypos2));
						Point p2(xpos,y1);
						Line temp(p1,p2);
						ans.push_back(temp);
						flag = 1;
						break;
					}
				}
			}
			if(flag == 0) ans.push_back(l);
		}else if(l.endpoint1.y == l.endpoint2.y){ //horizontal line
			int xpos1 = l.endpoint1.x;
			int xpos2 = l.endpoint2.x;
			int ypos = l.endpoint1.y;
			int flag = 0;
			for(Line l1:lines){
				int y1 = l1.endpoint1.y;
				int y2 = l1.endpoint2.y;
				if(y1 == y2) continue;
				int x1 = l1.endpoint1.x;
				if(ypos >= min(y1,y2) && ypos <= max(y1,y2)){
					if(max(xpos1,xpos2) <= x1 && x1-max(xpos1,xpos2) <= 1000){
						Point p1(min(xpos1,xpos2),ypos);
						Point p2(x1,ypos);
						Line temp(p1,p2);
						ans.push_back(temp);
						flag = 1;
						break;
					} else if(min(xpos1,xpos2) >= x1 && min(xpos1,xpos2)-x1 <= 1000){
						Point p1(max(xpos1,xpos2),ypos);
						Point p2(x1,ypos);
						Line temp(p1,p2);
						ans.push_back(temp);
						flag = 1;
						break;
					}
				}
			}
			if(flag == 0) ans.push_back(l);	
		}
	}
	return ans;
} 

//ɾ�����߶�
vector<Line> deleteShortLine(vector<Line>& lines){
	vector<Line> ans;
	for(Line l:lines){
		if(l.endpoint1.x == l.endpoint2.x && abs(l.endpoint1.y - l.endpoint2.y) < 1000) continue;
		if(l.endpoint1.y == l.endpoint2.y && abs(l.endpoint1.x - l.endpoint2.x) < 1000) continue;
		ans.push_back(l);
	}
	return ans;
} 

//�����ǵ���
vector<Point> createPoints(int minX, int minY, int maxX, int maxY, int dist, vector<Line>& lines, int type){ //��distΪ����������� 
	vector<Point> points;
	Point p1(maxX,maxY);
	Point p2(minX,minY);
	Point p3(minX,maxY);
	Point p4(maxX,minY);

	for(int i = minX + 1000; i < maxX; i += dist){
		for(int j = minY + 1000; j < maxY; j += dist){
			//����������
			/* 
			if(i >= 195444 && j >= 15816 && j <= 24951){
				//cout<<"pass"<<endl;
				continue;
			} 
			if(i >= 197374 && j >= 10281){
				continue;
			}
			
			//��������ܲ����豸
			
			if(type == 1 && i >= 194724 && j >= 40746){
				continue;
			}*/ 
			
			
			Point pos(i,j);
			
			if(!hasBarrier(pos,p1,lines) || !hasBarrier(pos,p2,lines) || !hasBarrier(pos,p3,lines) || !hasBarrier(pos,p4,lines)){
				continue;
			}
			if(isOnLine(pos,lines)) continue;
			if(type == 1 && isNearLine(pos,lines)) continue;
			points.push_back(pos);
		}
	}
	return points;
}


//ȷ��ÿ����ѡ������ܹ����ǵ�������㼯�� 
//dist1�����ϰ���ʱ��������Զ���룬dist2�����ϰ���ʱ��������Զ���� 
void getRelations(vector<Point>& points, vector<Point>& cands, vector<Line>& lines, int dist1, int dist2, vector<vector<int>>& cover_points, vector<vector<int>>& cover_sets){
	for(int c = 0; c < cands.size(); ++c){
		for(int p = 0; p < points.size(); ++p){
			double dist = sqrt(pow((cands[c].x - points[p].x),2) + pow((cands[c].y - points[p].y), 2));
			if(dist > dist1) continue;
			if(dist <= dist2 || !hasBarrier(cands[c], points[p], lines)){
				cover_points[c].push_back(p);
				cover_sets[p].push_back(c);
			}	
		}
	}						
} 

//ȷ��ÿ����ѡ������ܹ����ǵ�������㼯��
//dist1�����ϰ���ʱ��������Զ���룬dist2��ÿ����һ���ϰ������ͼ���dist2 
void getRelations1(vector<Point>& points, vector<Point>& cands, vector<Line>& lines, int dist1, int dist2, vector<vector<int>>& cover_points, vector<vector<int>>& cover_sets){
	for(int c = 0; c < cands.size();++c){
		for(int p = 0;p < points.size();++p){
			double dist = sqrt(pow((cands[c].x - points[p].x),2) + pow((cands[c].y - points[p].y), 2));
			if(dist > dist1) continue;
			int bcnt = barrierNum(cands[c],points[p],lines);
			int farthest = dist1 - bcnt*dist2 >= 0 ? dist1 - bcnt*dist2 : 0;
			if(dist > farthest) continue;
			cover_points[c].push_back(p);
			cover_sets[p].push_back(c);
		}
	}
}

//ȷ��ÿ����ѡ������ܹ����ǵ�������㼯��
//dist1�����ϰ���ʱ��������Զ���룬dist2��ÿ����һ���ϰ������ͼ���dist2 ���������ϰ���ܽ�������һ��
 void getRelations2(vector<Point>& points, vector<Point>& cands, vector<Line>& lines, int dist1, int dist2, vector<vector<int>>& cover_points, vector<vector<int>>& cover_sets){
	for(int c = 0; c < cands.size();++c){
		for(int p = 0;p < points.size();++p){
			double dist = sqrt(pow((cands[c].x - points[p].x),2) + pow((cands[c].y - points[p].y), 2));
			if(dist > dist1) continue;
			int bcnt = barrierNum1(cands[c],points[p],lines);
			int farthest = dist1 - bcnt*dist2 >= 0 ? dist1 - bcnt*dist2 : 0;
			if(dist > farthest) continue;
			cover_points[c].push_back(p);
			cover_sets[p].push_back(c);
		}
	}
}

//Լ��2������̫������������㲻��ͬʱ��ѡ�� 
vector<pair<int,int>> getConflictCand(vector<Point>& cands, double thre){
	vector<pair<int,int>> ans;
	for(int i = 0;i < cands.size();++i){
		for(int j = i+1;j < cands.size();++j){
			double dist = sqrt(pow((cands[i].x - cands[j].x),2) + pow((cands[i].y - cands[j].y), 2));
			if(dist <= thre){
				pair<int,int> p(i,j);
				ans.push_back(p);
			}
		}
	}
	return ans;
}

//����㷨��������
void writeFile(vector<vector<int>>& cover_sets, int m, int n){
	/*
	ofstream location_out;
	location_out.open("../data/testfile",std::ios::out | std::ios::app);
	if(!location_out.is_open())
		return;
	*/
	ofstream location_out("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/testfile");
	location_out << m << ' ' << n << endl;
	for(int i = 0;i < cover_sets.size(); ++i){
		vector<int> pos = cover_sets[i];
		int len = pos.size();
		//if(len == 0) continue;
		location_out<<len<<endl;
		for(int num:pos) location_out<<num+1<<' ';
		location_out<<endl;
	}
} 

//����㷨��������2 
void writeFile1(vector<vector<int>>& cover_sets, int m, int n, vector<Point>& cands,int cover_times, int thre){
	//ofstream location_out("../data/newtestfile");
	//ofstream location_out("./testfile"); //!
	ofstream location_out("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/newtestfile");
	
	//Լ��������̫������������㲻��ͬʱ��ѡ��
	vector<pair<int,int>> ans = getConflictCand(cands,thre); //5000��5��
	int total_constraints = m+n+ans.size();
	//int total_constraints = m+n; //!
	location_out << "p wcnf " << n << ' ' << total_constraints << ' ' << 10 << endl;
	for(int i = 0;i < cover_sets.size();++i){
		location_out<<"10 "<<cover_times<<" ";
		vector<int> set = cover_sets[i];
		for(int num:set) location_out<<num+1<<' ';
		location_out<<"0"<<endl;
	}
	
	for(int i = 0;i < n;++i){
		int num = -(i+1);
		location_out<<"1 1 "<<num<<" 0"<<endl;
	}
	
	
	for(auto elem:ans){
		int num1 = -(elem.first+1);
		int num2 = -(elem.second+1);
		location_out<<"1 1 "<<num1<<" "<<num2<<" 0"<<endl;
	} 	
	
} 

void writeResult(vector<int>& ans, vector<vector<int>>& cover_points,vector<Point>& cands,vector<Point>& points){
	ofstream location_out("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/resPoints");
	/*
	location_out.open("../data/resPoints",std::ios::out | std::ios::app);
	if(!location_out.is_open())
		return;
	*/
	vector<int> flag(points.size(),0);
	for(int num:ans){
		location_out<<cover_points[num].size()<<endl;
		location_out<<cands[num].x<<" "<<cands[num].y<<endl;
		for(int pos:cover_points[num]){
			if(flag[pos] == 1) continue;
			location_out<<points[pos].x<<" "<<points[pos].y<<endl;
			flag[pos] = 1;
		}
	}
}

void writeMergedLines(vector<Line>& lines){
	//ofstream location_out("../data/mergedLines.csv");
	ofstream location_out("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/mergedLines.csv");
	/*
	location_out.open("../data/mergedLines1.csv",std::ios::out | std::ios::app);
	if(!location_out.is_open())
		return;
	*/
	for(Line l:lines){
		location_out<<l.endpoint1.x<<" "<<l.endpoint2.x<<" "<<l.endpoint1.y<<" "<<l.endpoint2.y<<endl;
	}
}

bool checkCrossed(){
	Point p1(185934,3921);
	Point p2(184934,54266);
	Line l1(p1,p2);
	Point p3(184934,17816);
	Point p4(186524,17816);
	Line l2(p3,p4);
	if(isCrossed(l1,l2)) cout<<"crossed"<<endl;
	else cout<<"not crossed"<<endl;
	
}

//�ֶ������ 
vector<int> getManualRes(string filepath, vector<Point>& cands){
	cout<<"get Res Manually"<<endl;
	vector<vector<int>> ans;
	ifstream fin(filepath);
	string line;
	//��ȡ�ֶ�������������Ϣ 
	while(getline(fin,line)){
		istringstream sin(line);
		string field;
		vector<int> elem(2,0);
		int pos = 0;
		while(getline(sin,field,' ')){
			elem[pos++] = stoi(field);
		}	
		ans.push_back(elem);
	}
	vector<int> resPoints;
	for(int i = 0;i < ans.size();++i){
		int cur_x = ans[i][0];
		int cur_y = ans[i][1];
		for(int j = 0;j < cands.size();++j){
			if(int((cands[j].x - 184934)/1000) == cur_x && int((cands[j].y - 2746)/1000) == cur_y){
				resPoints.push_back(j);
				break;
			}
		}
	}
	return resPoints;	
}

void verifyManualRes(vector<int>& resPoints, vector<vector<int>>& cover_sets, int m, int cover_num){
	vector<int> flag(m,0);
	for(int i = 0;i < resPoints.size();++i){
		flag[resPoints[i]] = 1;
	}
	int ans = 0;
	for(int i = 0;i < cover_sets.size();++i){
		int len = cover_sets[i].size();
		int cnt = 0;
		for(int j = 0;j < len;++j){
			int s = cover_sets[i][j];
			cnt += flag[s];
		}
		if(cnt < cover_num) ans++;
	}
	cout<<endl;
	cout<<ans<<" points <"<<cover_num<<" Aps";
}

 
int main(int argc, char *argv[]){
	// Ҫ��ȡ���������ͣ�0-�������������Լ��㷨������������ 1-�㷨�����Ӧ�Ľڵ�����
	int data_type = atoi(argv[1]);
	int spread_dist = atoi(argv[2]);
	int reduce_dist = atoi(argv[3]);
	int cover_num = atoi(argv[4]);
	int dist_thre = atoi(argv[5]);
	//��ȡ�߶�
	vector<Line> lines = readLines("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/linesdata.csv");
	vector<int> border = getBorder(lines);
	for(int num:border) cout<<num<<' ';
	cout<<endl;
	
	lines = mergeLines(lines);
	lines = deleteShortLine(lines);
	if(data_type == 0){
	    writeMergedLines(lines);
	}
	//��㣨�ذ壬����������㣩 
	vector<Point> points = createPoints(border[0],border[2],border[1],border[3],1000,lines,0);
	//cout<<points.size()<<endl;

	//��㣨�컨�壬�豸��ѡ����㣩
	vector<Point> cands = createPoints(border[0],border[2],border[1],border[3],1000,lines,1);
	//cout<<cands.size()<<endl;	
	//�õ����ǹ�ϵ
	vector<vector<int>> cover_points(cands.size());
	vector<vector<int>> cover_sets(points.size());
	//getRelations(points, cands, lines, 20000, 15000, cover_points, cover_sets);
	getRelations2(points, cands, lines, spread_dist, reduce_dist, cover_points, cover_sets);
	//cout<<cover_points.size()<<endl;
	//cout<<cover_sets.size()<<endl;
	/*for check
	for(int i = 0;i < 3;++i){
		for(int num:cover_points[i]) cout<<num<<' ';
		cout<<endl;
	}*/
	cout<<"total cands:"<<cover_points.size()<<" total points:"<<cover_sets.size()<<endl;
	if(data_type == 0){
	    //���scp�㷨��������
	    writeFile1(cover_sets,points.size(),cands.size(),cands,cover_num,dist_thre); //new_code
	} else if (data_type == 1){
	    //��������Ϣ
	    vector<int> ans = readResult("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/solution.res");
	    for(int num:ans) cout<<num<<' ';
	    cout<<endl;
	    writeResult(ans,cover_points,cands,points);
	    cout<<"success"<<endl;
	} else if(data_type == 2){
		//vector<int> resPoints = getManualRes("./codeRes_2.txt", cands);
		vector<int> resPoints = getManualRes("./manualRes.txt", cands);
		writeResult(resPoints,cover_points,cands,points);
		for(int num:resPoints) cout<<num<<" :"<<cands[num].x-184934<<" "<<cands[num].y-2746<<endl;
		verifyManualRes(resPoints,cover_sets,cands.size(),1);
		verifyManualRes(resPoints,cover_sets,cands.size(),2);
		verifyManualRes(resPoints,cover_sets,cands.size(),3);
		verifyManualRes(resPoints,cover_sets,cands.size(),4);
		verifyManualRes(resPoints,cover_sets,cands.size(),5);
	}
}




