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
	int type;
	Line(Point p1, Point p2, int lines_type = 0): endpoint1(p1), endpoint2(p2), type(lines_type){}
};

//读取线段信息 
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

//读取线段信息 （含障碍物类型）
vector<vector<Line>> readLinesByType(string filepath, int type_cnt){
	//vector<Line> lines;
	ifstream fin(filepath);
	cout<<filepath<<endl;
	string line;
	vector<vector<Line>> lines_by_type(type_cnt);
	while(getline(fin,line)){
		//cout<<line<<endl;
		istringstream sin(line);
		vector<int> fields(5,0);
		string field;
		int i = 0;
		while(getline(sin,field,',')){
			fields[i++] = stoi(field);
		}
		Point p1(fields[0],fields[1]);
		Point p2(fields[2],fields[3]);
		int type = fields[4];
		Line curline(p1, p2, type);
		//lines.push_back(curline);
		lines_by_type[type].push_back(curline);
	}
	return lines_by_type;
}

//读取线段信息 
vector<Line> readLinesByType1(string filepath){
	vector<Line> lines;
	ifstream fin(filepath);
	cout<<filepath<<endl;
	string line;
	while(getline(fin,line)){
		//cout<<line<<endl;
		istringstream sin(line);
		vector<int> fields(5,0);
		string field;
		int i = 0;
		while(getline(sin,field,',')){
			fields[i++] = stoi(field);
		}
		Point p1(fields[0],fields[1]);
		Point p2(fields[2],fields[3]);
		Line curline(p1,p2,fields[4]);
		lines.push_back(curline);	
	}
	return lines;
}

void checkLineType(vector<Line>& lines){
	for(Line l:lines){
		if(l.type > 3){
			cout<<"wrong type:"<<l.type<<endl;
		}
	}
	cout<<"no wrong type"<<endl;
}

bool checkLineType1(vector<Line>& lines){
	for(Line l:lines){
		if(l.type > 3){
			return false;
		}
	}
	return true;
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
//获取边界
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



//判断两条线段是否相交 
bool isCrossed(Line line1, Line line2){
	//参考https://www.cnblogs.com/wuwangchuxin0924/p/6218494.html 
	long line1_x1 = line1.endpoint1.x;
	long line1_x2 = line1.endpoint2.x;
	long line2_x1 = line2.endpoint1.x;
	long line2_x2 = line2.endpoint2.x;
	long line1_y1 = line1.endpoint1.y;
	long line1_y2 = line1.endpoint2.y;
	long line2_y1 = line2.endpoint1.y;
	long line2_y2 = line2.endpoint2.y;
	
	//x轴上不相交则不相交，返回false 
	if(min(line1_x1,line1_x2) > max(line2_x1,line2_x2) || min(line2_x1,line2_x2) > max(line1_x1,line1_x2)){
		return false;
	}
	//y轴上不相交则不相交，返回false 
	if(min(line1_y1,line1_y2) > max(line2_y1,line2_y2) || min(line2_y1,line2_y2) > max(line1_y1,line1_y2)){
		return false;
	}
	//继续用向量相乘来判断
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

//判断两点之间有无障碍物
bool hasBarrier(Point p1, Point p2, vector<Line>& lines){
	Line curline(p1,p2);
	for(Line l:lines){
		if(isCrossed(curline,l)) return true;
	}
	return false;
} 

//判断两点之间障碍物的数量 
int barrierNum(Point p1, Point p2, vector<Line>& lines){
	Line curline(p1,p2);
	int cnt = 0;
	for(Line l:lines){
		if(isCrossed(curline,l)) cnt++;
	}
	return cnt;
}

//判断两点之间障碍物的数量 
vector<int> barrierNumByType(Point p1, Point p2, vector<Line>& lines,int type_cnt){
	Line curline(p1,p2);
	vector<int> cnt(type_cnt,0);
	for(Line l:lines){
		if(isCrossed(curline,l)) cnt[l.type]++;
	}
	return cnt;
}

//垂直线间的排序，按横轴排序 
bool vlinesCompare(Line const & l1, Line const & l2){
	return l1.endpoint1.x < l2.endpoint1.x;
}

//水平线间的排序，按纵轴排序
bool hlinesCompare(Line const & l1, Line const & l2){
	return l1.endpoint1.y < l2.endpoint1.y;
} 

//判断两点之间障碍物的数量，若障碍物距离很近，当作一个 
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
	//排序
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

//判断两点之间障碍物的数量，若障碍物距离很近，当作一个
vector<int> barrierNum1ByType(Point p1, Point p2, vector<Line>& lines, int type_cnt){
	Line curline(p1,p2);
	vector<Line> h_lines;
	vector<Line> v_lines;
	if(!checkLineType1(lines)) cout<<"has illegal line"<<endl;
	for(Line l:lines){
		if(isCrossed(curline,l)){
			if(l.endpoint1.x == l.endpoint2.x){
				if(l.type > 3) cout<<"illegal line"<<endl;
				v_lines.push_back(l);
			} else if(l.endpoint1.y == l.endpoint2.y){
				if(l.type > 3) cout<<"illegal line"<<endl;
				h_lines.push_back(l);
			}
		}
	}
	if(!checkLineType1(v_lines)) cout<<"before sort, has illegal vertical line"<<endl;
	if(!checkLineType1(h_lines)) cout<<"before sort, has illegal h_line"<<endl;
	//排序
	sort(v_lines.begin(),v_lines.end(),vlinesCompare);
	sort(h_lines.begin(),h_lines.end(),hlinesCompare);
	if(!checkLineType1(v_lines)) cout<<"after sort, has illegal vertical line"<<endl;
	if(!checkLineType1(h_lines)) cout<<"after sort, has illegal h_line"<<endl;
	vector<int> ans(type_cnt,0);
	if(!checkLineType1(lines)) cout<<"has illegal line"<<endl;
	if(v_lines.size() > 0){
		int last = 0;
		ans[v_lines[last].type]++;
		for(int i = 1;i < v_lines.size();++i){
			if(v_lines[i].endpoint1.x - v_lines[last].endpoint1.x > 500){
				if(v_lines[last].type > 3) cout<<"wrong vertical:"<<v_lines[last].type<<endl;
				last = i;
				ans[v_lines[last].type]++;
			}
		}
	}
	if(h_lines.size() > 0){
		int last = 0;
		ans[h_lines[last].type]++;
		for(int i = 1;i < h_lines.size();++i){
			if(h_lines[i].endpoint1.y - h_lines[last].endpoint1.y > 500){
				if(h_lines[last].type > 3) cout<<"wrong h："<<h_lines[last].type<<endl;
				last = i;
				ans[h_lines[last].type]++;
			}
		}
	}
	return ans;
}

//判断点是否在线段上
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

//判断点是否在线段周围
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

bool myCompare1(vector<int>& a, vector<int>& b){
	return a[0] < b[0];
}


//处理断断续续的线段
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
	//处理水平线 
	for(iter = h_lines.begin(); iter != h_lines.end(); ++iter){
		int y = iter->first;
		vector<pair<int,int>> elem = iter->second;
		sort(elem.begin(),elem.end(),myCompare);
		int begin_pos = 0;
		int i = 0;
		while(i < elem.size()){
			if(i < elem.size()-1 && elem[i+1].first - elem[i].second <= 2000 && elem[i+1].first - elem[i].second >= 0) i++;
			else{
				//check是否重复
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
	//处理垂直线
	for(iter = v_lines.begin(); iter != v_lines.end(); ++iter){
		int x = iter->first;
		vector<pair<int,int>> elem = iter->second;
		sort(elem.begin(),elem.end(),myCompare);
		int begin_pos = 0;
		int i = 0;
		while(i < elem.size()){
			if(i < elem.size()-1 && elem[i+1].first - elem[i].second <= 2000 && elem[i+1].first - elem[i].second >= 0) i++;
			else{
				//check是否重复
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

//处理断断续续的线段（考虑障碍物类型）
vector<Line> mergeLinesByType(vector<Line>& lines){
	map<int, vector<vector<int>>> h_lines;
	map<int, vector<vector<int>>> v_lines;
	//checkLineType(lines);
	for(Line l:lines){
		int x1 = l.endpoint1.x;
		int x2 = l.endpoint2.x;
		int y1 = l.endpoint1.y;
		int y2 = l.endpoint2.y;
		if(x1 == x2){
			vector<int> p(3,0);
			p[0] = min(y1,y2);
			p[1] = max(y1,y2);
			p[2] = l.type;
			v_lines[x1].push_back(p);
		}else if(y1 == y2){
			vector<int> p(3,0);
			p[0] = min(x1,x2);
			p[1] = max(x1,x2);
			p[2] = l.type;
			h_lines[y1].push_back(p);
		}
	}
	vector<Line> new_lines;
	map<int, vector<vector<int>>>::iterator iter;
	//处理水平线
	for(iter = h_lines.begin(); iter != h_lines.end(); ++iter){
		int y = iter->first;
		vector<vector<int>> elem = iter->second;
		sort(elem.begin(),elem.end(),myCompare1);
		int begin_pos = 0;
		int i = 0;
		while(i < elem.size()){
			if(i < elem.size()-1 && elem[i+1][0] - elem[i][1] <= 2000 && elem[i+1][0] - elem[i][1] >= 0) i++;
			else{
				//check是否重复
				int k = new_lines.size()-1;
				int flag = 0;
				while(k >= 0 && new_lines[k].endpoint1.y == y){
					if(new_lines[k].endpoint1.x == elem[begin_pos][0] && new_lines[k].endpoint2.x == elem[i][1]){
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
				Point p1(elem[begin_pos][0], y);
				Point p2(elem[i][1], y);
				int type = 0;
				Line temp(p1,p2,type);
				new_lines.push_back(temp);
				begin_pos = i+1;
				i = begin_pos;
			}
		}
	}
	//处理垂直线
	for(iter = v_lines.begin(); iter != v_lines.end(); ++iter){
		int x = iter->first;
		vector<vector<int>> elem = iter->second;
		sort(elem.begin(),elem.end(),myCompare1);
		int begin_pos = 0;
		int i = 0;
		while(i < elem.size()){
			if(i < elem.size()-1 && elem[i+1][0] - elem[i][1] <= 2000 && elem[i+1][0] - elem[i][1] >= 0) i++;
			else{
				//check是否重复
				int k = new_lines.size()-1;
				int flag = 0;
				while(k >= 0 && new_lines[k].endpoint1.x == x){
					if(new_lines[k].endpoint1.y == elem[begin_pos][0] && new_lines[k].endpoint2.y == elem[i][1]){
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
				Point p1(x, elem[begin_pos][0]);
				Point p2(x, elem[i][1]);
				int type = 0;
				Line temp(p1,p2,type);
				new_lines.push_back(temp);
				begin_pos = i+1;
				i = begin_pos;
			}
		}
	}
	return new_lines;
}
/*
vector<Line> mergeLinesByType1(vector<vector<Line>>& lines_by_type){
    vector<Line> lines;
    for(int i = 0;i < lines_by_type.size();++i){
        vector<Line> partial_lines = mergeLinesByType(lines_by_type[i],i);
        lines.insert(lines.end(),partial_lines.begin(),partial_lines.end());
    }
    return lines;
} */

//处理缺口
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

//删除短线段
vector<Line> deleteShortLine(vector<Line>& lines){
	vector<Line> ans;
	for(Line l:lines){
		if(l.endpoint1.x == l.endpoint2.x && abs(l.endpoint1.y - l.endpoint2.y) < 1000) continue;
		if(l.endpoint1.y == l.endpoint2.y && abs(l.endpoint1.x - l.endpoint2.x) < 1000) continue;
		ans.push_back(l);
	}
	return ans;
} 

//待覆盖点打点
vector<Point> createPoints(int minX, int minY, int maxX, int maxY, int dist, vector<Line>& lines, int type){ //以dist为间隔划分网格 
	vector<Point> points;
	Point p1(maxX,maxY);
	Point p2(minX,minY);
	Point p3(minX,maxY);
	Point p4(maxX,minY);

	for(int i = minX + 1000; i < maxX; i += dist){
		for(int j = minY + 1000; j < maxY; j += dist){
			//测试区域处理
			/* 
			if(i >= 195444 && j >= 15816 && j <= 24951){
				//cout<<"pass"<<endl;
				continue;
			} 
			if(i >= 197374 && j >= 10281){
				continue;
			}
			
			//这个区域不能部署设备
			
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


//确定每个候选部署点能够覆盖到的坐标点集合 
//dist1：无障碍物时传播的最远距离，dist2：有障碍物时传播的最远距离 
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

//确定每个候选部署点能够覆盖到的坐标点集合
//dist1：无障碍物时传播的最远距离，dist2：每遇到一次障碍物，距离就减少dist2 
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

//确定每个候选部署点能够覆盖到的坐标点集合
//dist1：无障碍物时传播的最远距离，dist2：每遇到一次障碍物，距离就减少dist2 ，若两个障碍物很近，当作一个
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

//确定每个候选部署点能够覆盖到的坐标点集合
//dist1：无障碍物时传播的最远距离，dist2：每遇到一次障碍物，距离就减少dist2 ，若两个障碍物很近，当作一个
 void getRelationsByType(vector<Point>& points, vector<Point>& cands, vector<Line>& lines, int dist1, vector<vector<int>>& cover_points, vector<vector<int>>& cover_sets, vector<int>& reduceDist){
	checkLineType(lines);
	for(int c = 0; c < cands.size();++c){
		for(int p = 0;p < points.size();++p){
			double dist = sqrt(pow((cands[c].x - points[p].x),2) + pow((cands[c].y - points[p].y), 2));
			if(dist > dist1) continue;
			//cout<<"here 1"<<endl;
			vector<int> bcnt = barrierNum1ByType(cands[c],points[p],lines,reduceDist.size());
			//cout<<"here 2"<<endl;
			int total_reduce_dist = 0;
			for(int i = 0;i < reduceDist.size();++i){
			    total_reduce_dist += reduceDist[i]*bcnt[i];
			}
			//cout<<"total_reduce_dist"<<endl;
			int farthest = dist1 - total_reduce_dist >= 0 ? dist1 - total_reduce_dist : 0;
			if(dist > farthest) continue;
			cover_points[c].push_back(p);
			cover_sets[p].push_back(c);
		}
	}
	cout<<"success"<<endl;
}

//约束2：距离太近的两个部署点不能同时被选中 
vector<vector<int>> getConflictCand(vector<Point>& cands, double thre){
	vector<vector<int>> ans;
	for(int i = 0;i < cands.size();++i){
		for(int j = i+1;j < cands.size();++j){
			double dist = sqrt(pow((cands[i].x - cands[j].x),2) + pow((cands[i].y - cands[j].y), 2));
			if(dist <= thre){
				vector<int> p(3,0);
				p[0] = i;
				p[1] = j;
				p[2] = ceil(dist);
				ans.push_back(p);
			}
		}
	}
	return ans;
}

//输出算法所需数据
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

//输出算法所需数据2 
void writeFile1(vector<vector<int>>& cover_sets, int m, int n, vector<Point>& cands,int cover_times, int thre){
	ofstream location_out("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/newtestfile");
	
	//约束：距离太近的两个部署点不能同时被选中
	vector<vector<int>> conflictPos = getConflictCand(cands,thre); //5000：5米
	int total_constraints = m+n+conflictPos.size();
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
	
	for(int i = 0;i < conflictPos.size();++i){
		int num1 = -(conflictPos[i][0] + 1);
		int num2 = -(conflictPos[i][1] + 1);
		int dist = conflictPos[i][2];
		if(dist <= 5000){
			location_out<<"10 1 "<<num1<<" "<<num2<<" 0"<<endl;
		} else{
			location_out<<"5 1 "<<num1<<" "<<num2<<" 0"<<endl;
		}
	}
	/*
	for(auto elem:ans){
		int num1 = -(elem.first+1);
		int num2 = -(elem.second+1);
		
		location_out<<"8 1 "<<num1<<" "<<num2<<" 0"<<endl;
	} 	
	*/
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
	ofstream location_out("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/mergedLines.csv");
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

//手动构造解 
vector<int> getManualRes(string filepath, vector<Point>& cands){
	cout<<"get Res Manually"<<endl;
	vector<vector<int>> ans;
	ifstream fin(filepath);
	string line;
	//获取手动构造的坐标点信息 
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
	// 要获取的数据类型：0-线条处理数据以及算法输入所需数据 1-解中对应的节点数据，2-确认人工构造的解
	int data_type = atoi(argv[1]);
	int spread_dist = atoi(argv[2]);
	int cover_num = atoi(argv[3]);
	int dist_thre = atoi(argv[4]);
	int wall_reduce_dist = atoi(argv[5]);
	int glass_reduce_dist = atoi(argv[6]);
	int wood_reduce_dist = atoi(argv[7]);
	int other_reduce_dist = atoi(argv[8]);

	int reduce_dist = wall_reduce_dist;
	//记录不同类型的材料的衰减距离
	vector<int> reduceDist;
	reduceDist.push_back(wall_reduce_dist);
	reduceDist.push_back(glass_reduce_dist);
	reduceDist.push_back(wood_reduce_dist);
	reduceDist.push_back(other_reduce_dist);
	int type_cnt = reduceDist.size();
	//读取线段
	/*
	vector<Line> lines = readLines("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/linesdata.csv");
	cout<<lines.size()<<endl;
	lines = mergeLines(lines);
	cout<<lines.size()<<endl;*/
	
	
	//vector<vector<Line>> lines_by_type = readLinesByType("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/linesDataByMaterials.csv",type_cnt);
	vector<Line> lines = readLinesByType1("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/linesDataByMaterials.csv");
	//cout<<lines_by_type[0].size()<<" "<<lines_by_type[1].size()<<" "<<lines_by_type[2].size()<<" "<<lines_by_type[3].size()<<endl;
	//vector<Line> lines = mergeLinesByType(lines_by_type); 
	checkLineType(lines);
	lines = mergeLinesByType(lines); 
	checkLineType(lines);
	cout<<lines.size()<<endl;
	
	
	lines = deleteShortLine(lines);
	if(data_type == 0){
	    writeMergedLines(lines);
	}
	vector<int> border = getBorder(lines);
	for(int num:border) cout<<num<<' ';
	cout<<endl;
	//打点（地板，待覆盖坐标点） 
	vector<Point> points = createPoints(border[0],border[2],border[1],border[3],1000,lines,0);

	//打点（天花板，设备候选部署点）
	vector<Point> cands = createPoints(border[0],border[2],border[1],border[3],1000,lines,1);
	//得到覆盖关系
	vector<vector<int>> cover_points(cands.size());
	vector<vector<int>> cover_sets(points.size());
	//getRelations2(points, cands, lines, spread_dist, reduce_dist, cover_points, cover_sets);
	getRelationsByType(points, cands, lines, spread_dist, cover_points, cover_sets, reduceDist);
	cout<<"total cands:"<<cover_points.size()<<" total points:"<<cover_sets.size()<<endl;
	if(data_type == 0){
	    //输出scp算法所需数据
	    writeFile1(cover_sets,points.size(),cands.size(),cands,cover_num,dist_thre); //new_code
	} else if (data_type == 1){
	    //输出解的信息
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




