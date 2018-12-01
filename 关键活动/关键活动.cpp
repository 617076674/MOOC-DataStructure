#include<iostream>
#include<vector>
#include<queue>
#include<set>
#include<algorithm>

using namespace std;

struct node {
	int v;
	int len;
	node(int _v, int _len) {
		v = _v;
		len = _len;
	}
};

struct activity {
	int startV, endV;
	activity(int _startV, int _endV) {
		startV = _startV;
		endV = _endV;
	}
};

int N, M, inDegree[101], d[101], INF = 1000000000, countInq[101], resultMinTime = INF, startPoint;
vector<node> graph[101];	//�ڽӱ���ʽ�洢ͼ 
bool inq[101];
vector<int> tempPath;
set<int> pre[101];
int inputSequence[101][101];	//��¼ÿ��������ʱ��˳��
vector<activity> resultActivities;	//����ؼ�·���ϵı���Ϣ 

void init();
bool spfa(int s);
void dfs(int nowVisit, bool visited[]);
bool cmp(activity a1, activity a2);

int main() {
	scanf("%d %d", &N, &M);	//��ȡ�ڵ����ͱ��� 
	bool visited[M];	//��ǵ�i�����Ƿ��Ѿ��ڽ������ 
	fill(inDegree + 1, inDegree + N + 1, 0);
	int v1, v2, len;
	for(int i = 0; i < M; i++) {
		scanf("%d %d %d", &v1, &v2, &len);
		graph[v1].push_back(node(v2, -len));
		inDegree[v2]++;
		inputSequence[v1][v2] = i;
	}
	for(int i = 1; i <= N; i++) {
		if(inDegree[i] == 0) {	//���Ϊ0�ĵ������
			init();
			startPoint = i;	//�Ե�iΪ���
			bool flag = spfa(startPoint);	//�����i�����������·��
			if(flag) {
				int minTime = INF;
				for(int j = 1; j <= N; j++) {
					if(j != i && d[j] < minTime) {	//Ѱ�Ҵӵ�i�����������·���е���Сֵ
						minTime = d[j];	//������Сʱ����Ϣ
					}
				}
				if(minTime < resultMinTime) {	//�����iΪ��㵽���������·������Сֵ�ȵ�ǰ����С·����ҪС
					resultMinTime = minTime;	//������С·����Ϣ
					resultActivities.clear();	//���resultActivities
					fill(visited, visited + M, false);	//������нڵ�δ������ 
					for(int j = 1; j <= N; j++) {
						if(j != i && d[j] == minTime) {
							dfs(j, visited);
						}
					}
				} else if(minTime == resultMinTime) {
					for(int j = 1; j <= N; j++) {
						if(j != i && d[j] == minTime) {
							dfs(j, visited);
						}
					}
				}
			}
		}
	}
	if(resultMinTime == INF) {
		printf("0\n");
		return 0;
	}
	printf("%d\n", -resultMinTime);	//����ؼ�·������ 
	sort(resultActivities.begin(), resultActivities.end(), cmp);	//�Խ���߽������� 
	for(int i = 0; i < resultActivities.size(); i++) {	//������ 
		printf("%d->%d\n", resultActivities[i].startV, resultActivities[i].endV);
	}
	return 0;
}

void init() {
	fill(d + 1, d + N + 1, INF);
	fill(inq + 1, inq + N + 1, false);
	fill(countInq + 1, countInq + N + 1, 0);
	for(int j = 1; j <= N; j++) {
		pre[j].clear();
	}
}

bool spfa(int s) {
	d[s] = 0;
	queue<int> q;
	q.push(s);
	inq[s] = true;
	countInq[s]++;
	while(!q.empty()) {
		int u = q.front();
		q.pop();
		inq[u] = false;
		for(int i = 0; i < graph[u].size(); i++) {
			int v = graph[u][i].v;
			int len = graph[u][i].len;
			if(d[u] + len < d[v]) {
				d[v] = d[u] + len;
				pre[v].clear();
				pre[v].insert(u);
				if(!inq[v]) {
					q.push(v);
					inq[v] = true;
					countInq[v]++;
					if(countInq[v] > N - 1) {
						return false;
					}
				}
			} else if(d[u] + len == d[v]) {
				pre[v].insert(u);
				if(!inq[v]) {
					q.push(v);
					inq[v] = true;
					countInq[v]++;
					if(countInq[v] > N - 1) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

void dfs(int nowVisit, bool visited[]) {
	tempPath.push_back(nowVisit);
	if(nowVisit == startPoint) {
		for(int i = tempPath.size() - 1; i > 0; i--) {
			int inputNumber = inputSequence[tempPath[i]][tempPath[i - 1]];
			if(!visited[inputNumber]) {
				resultActivities.push_back(activity(tempPath[i], tempPath[i - 1]));
				visited[inputNumber] = true;
			}
		}
		tempPath.pop_back();
		return;
	}
	for(set<int>::iterator it = pre[nowVisit].begin(); it != pre[nowVisit].end(); it++) {
		dfs(*it, visited);
	}
	tempPath.pop_back();
}

bool cmp(activity a1, activity a2) {
	if(a1.startV == a2.startV) {
		return inputSequence[a1.startV][a1.endV] > inputSequence[a2.startV][a2.endV];
	} else {
		return a1.startV < a2.startV;
	}
}
