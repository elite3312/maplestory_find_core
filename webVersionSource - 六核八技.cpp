#include <stdio.h>
#include <string.h>


struct Core{
	int skills[10];
	int skillOrder[3];
};
struct SkillOccur {
	int skill,occur;
};
void modeOne(struct Core core[], int coreCount, int enumerate,int priority[6]);
void modeTwo(struct Core core[], int coreCount, int enumerate, int priority[6]);
int main() {
	/*參數設定區*/
	char mode = '1';/*設定'1'找4核6技 ,'2'找 6核9技*/
	char enumerate = '2';/*設 '1' 列出所有解,設'2' 列第一組合乎成本解,設'3'列第一組解 */
	printf("歡迎使用楓之谷完美核心篩選器~~\n\n");
	/*請在陣列ｓ1中輸入您持有的核心，用來找四核六技，以','分隔*/
	char s1[1000] = "653,624,643,613,614,641,621,624,416,435,456,465,462,421,165,153,132,154,136,142,152,125,263,253,216,215,216,241,263,246,264,365,316,321"
		"316,316,362,345,341,361,354,512,534,512,532,536";
	/*請在陣列ｓ2中輸入您持有的核心，用來找六核九技，以','分隔*/
	char s2[1000] = "651,571,581,591,831,371,951,534,964,894,329,289,649,459,389,349,519,219,329,496,576,216,346,426,876,756,256,"
		"342,782,592,462,412,492,162,762,973,873,763,563,793,693,673,948,548,478,598,648,758,458,298,658,285,625,735,765,975,795,245,945,867,517,967,437,597,867,497,517";
	char *s;
	if (mode == '1')s=s1;
	else s=s2;

	struct Core core[1000];
	int coreCount = 0;
	char*p=s;
	while (1) {
		
		if (*p == 0)break;
		else if(*p==',')p++;
		for (int i = 0; i < 10; i++) {
			core[coreCount].skills[i] = 0;
		}
		for (int i = 0; i < 3; i++) {
			core[coreCount].skillOrder[i] = *(p)-48;
				core[coreCount].skills[*(p)-48] = 1;
				p++;
		}
		
		coreCount++;
	}
	/*data input done*/

	/*printing cores, recording subject distribution*/
	int a, b;
	struct SkillOccur stack[10];
	for (a = 0; a < 10; a++) { stack[a].occur = 0; stack[a].skill = a; }
	printf("列出擁有的核心...\n");
	for (a = 0; a < coreCount; a++) {

		printf("核心編號:%d, [ ", a + 1);

		for (b = 0; b < 3; b++) {
			printf("%d ", core[a].skillOrder[b]);
			if (b == 2) { stack[core[a].skillOrder[b]].occur += 1; stack[core[a].skillOrder[b]].skill = core[a].skillOrder[b]; }
		}
		printf("]\n");
	}
	/*bubblesort the stack*/
	struct SkillOccur temp;
	for (a = 1; a <= 8; a++) {
		for (b = 1; b <= 8 - (a-1); b++) {
			if (stack[b].occur > stack[b + 1].occur) {
				temp = stack[b+1];
				stack[b+1] = stack[b];
				stack[b] = temp;
			}
		}
	}
	/*列出主詞出現次數*/
	printf("列出主詞出現次數\n");
	b = 9;
	for (a = 1; a <= b; a++) printf("主詞%d出現%d\n",stack[a].skill,stack[a].occur);
	int priority[6];
	if (mode == '1')
		for (a = 0; a < 4; a++)
			priority[a] = stack[b - a].skill;
	else for (a = 0; a <6; a++)
		priority[a] = stack[b - a].skill;
	
	/*search*/
	if (mode == '1')modeOne(core, coreCount,enumerate,  priority);
	else modeTwo(core, coreCount, enumerate,  priority);
	
	
	return 0;
}
void modeOne(struct Core core[],int coreCount,int enumerate, int priority[6]) {
	/*
	finding core combinations using brute force loops
	*/
	int selected[7] = { 0 };
	int hold[4];
	int i, j, k, l,a,b;
	printf("start\n");
	for (i = 0; i < coreCount; i++) {
		
		for (a = 1; a <= 6; a++) {
			if (core[i].skills[a])
				selected[a] += 1;
		}
		hold[0] = i;/*selected 1 core*/
		for (j = 0; j < coreCount; j++) {
			if (j == i)continue;
			for (a = 1; a <= 6; a++) {
				if (core[j].skills[a])
					selected[a] += 1;
			}

			hold[1] = j;//selected 2 cores
			for (k = 0; k < coreCount; k++) {
				if (k == i || k == j)continue;
				int sentinel = 0;
				for (a = 1; a <= 6; a++) {
					if (core[k].skills[a] && 3 <= selected[a] + 1)/*if 3== selected[a] + 1, then reject subtree*/
					{
						sentinel = 1; break;
					}

				}
				if (sentinel)continue;
				for (a = 1; a <= 6; a++) {
					if (core[k].skills[a])
						selected[a] += 1;
				}

				hold[2] = k;//selected 3 cores
				for (l = 0; l < coreCount; l++) {
					if (l == i || l == j || l == k)continue;
					for (a = 1; a <= 6; a++) {
						if (core[l].skills[a])
							selected[a] += 1;
					}
					hold[3] = l;//selected 4 cores, checkpoint
					if (selected[1] == 2//checking condition
						&& selected[2] == 2
						&& selected[3] == 2
						&& selected[4] == 2
						&& selected[5] == 2
						&& selected[6] == 2
						) {
						//printf("\n找到一組4核六技!\n");
						int checkDup[4] = { 0,0,0,0 }, dupCount = 0;
						for (a = 0; a < 4; a++) {
							//	printf("核心編號:%d, [ ", hold[a] + 1);
							checkDup[dupCount] = core[hold[a]].skillOrder[2];
							dupCount++;

							//	printf("]\n\n");

						}
						if (checkDup[0] != checkDup[1] &&
							checkDup[0] != checkDup[2] &&
							checkDup[0] != checkDup[3] &&
							checkDup[1] != checkDup[2] &&
							checkDup[1] != checkDup[3] &&
							checkDup[2] != checkDup[3]
							) {
							int prefered = 0;
							/*compare priority and checkdup*/
							
							for (a = 0; a < 4; a++)
								for (b = 0; b < 4; b++)
									if (priority[a] == checkDup[b])
										prefered++;
							if (prefered == 4)
								printf("\n找到一組四核六技!，且它的主詞符合您持有數量前四多的主詞!(%d,%d,%d,%d)\n", priority[0], priority[1], priority[2], priority[3]);
							/*else 
								printf("\n找到一組四核六技!，但它的主詞不符合您持有數量前四多的主詞(%d,%d,%d,%d)", priority[0], priority[1], priority[2], priority[3]);*/
							
							if (prefered != 4 && enumerate == '2') {
								for (a = 1; a <= 6; a++) {
									if (core[l].skills[a])
										selected[a] -= 1;
								}
								continue;
							}
							printf("\n");

							for (a = 0; a < 4; a++) {
								printf("核心編號:%d, [ ", hold[a] + 1);

								for (b = 0; b < 3; b++) {

									printf("%d ", core[hold[a]].skillOrder[b]);


								}
								printf("]\n");

							}	
							
							if (enumerate == '2'&&prefered == 4) { return; }
							else if(enumerate == '3') { return; }
						}
						
					}
					for (a = 1; a <= 6; a++) {
						if (core[l].skills[a])
							selected[a] -= 1;
					}
				}
				for (a = 1; a <= 6; a++) {
					if (core[k].skills[a])
						selected[a] -= 1;
				}
			}
			for (a = 1; a <= 6; a++) {
				if (core[j].skills[a])
					selected[a] -= 1;
			}
		}
		for (a = 1; a <= 6; a++) {
			if (core[i].skills[a])
				selected[a] -= 1;
		}
	}
	printf("\ndone(若都沒有出現\"找到一組4核六技!\"字樣，表示沒有發現可能的組合，請再多蒐集一些核心!)\n");


 }
void modeTwo(struct Core core[], int coreCount, int enumerate, int priority[6]) {
	/*
	finding core combinations using brute force loops
	*/
	int selected[10] = { 0 };
	int hold[6];
	int i, j, k, l,m,n, a, b, sentinel;
	printf("start\n");
	for (i = 0; i < coreCount; i++) {
		for (a = 1; a <= 9; a++) {
			if (core[i].skills[a])
				selected[a] += 1;
		}
		hold[0] = i;//selected 1 core
		for (j = 0; j < coreCount; j++) {
			if (j == i)continue;
			for (a = 1; a <= 9; a++) {
				if (core[j].skills[a])
					selected[a] += 1;
			}
			hold[1] = j;//selected 2 cores
			for (k = 0; k < coreCount; k++) {
				if (k == i || k == j)continue;
				sentinel = 0;
				for (a = 1; a <= 9; a++) {
					if (core[k].skills[a] && 3 == selected[a] + 1)/*if 3== selected[a] + 1, then reject subtree*/
					{
						sentinel = 1; break;
					}

				}
				if (sentinel)continue;
				for (a = 1; a <= 9; a++) {
					if (core[k].skills[a])
						selected[a] += 1;
				}

				hold[2] = k;//selected 3 cores
				for (l = 0; l < coreCount; l++) {
					if (l == i || l == j||l==k)continue;
					sentinel = 0;
					for (a = 1; a <= 9; a++) {
						if (core[l].skills[a] && 3 == selected[a] + 1)/*if 3== selected[a] + 1, then reject subtree*/
						{
							sentinel = 1; break;
						}

					}
					if (sentinel)continue;
					for (a = 1; a <= 9; a++) {
						if (core[l].skills[a])
							selected[a] += 1;
					}

					hold[3] = l;//selected 4 cores
					for (m = 0; m < coreCount; m++) {
						if (m == i || m == j || m == k||m==l)continue;
						sentinel = 0;
						for (a = 1; a <= 9; a++) {
							if (core[m].skills[a] && 3 == selected[a] + 1)/*if 3== selected[a] + 1, then reject subtree*/
							{
								sentinel = 1; break;
							}

						}
						if (sentinel)continue;
						for (a = 1; a <= 9; a++) {
							if (core[m].skills[a])
								selected[a] += 1;
						}

						hold[4] = m;//selected 5 cores
						for (n = 0; n < coreCount; n++) {
							if (n == i || n == j || n == k||n==l||n==m)continue;
							for (a = 1; a <= 9; a++) {
								if (core[n].skills[a])
									selected[a] += 1;
							}
							hold[5] = n;//selected 6 cores
							if (selected[1] >= 2/*final check*/
								&& selected[2] >= 2
								&& selected[3] >= 2
								&& selected[4] >= 2
								&& selected[5] >= 2
								&& selected[6] >= 2
								&& selected[7] >= 2
								&& selected[8] >= 2
								
								) {
								//printf("\n找到一組6核8技!\n");
								int checkDup[6] = { 0 }, dupCount = 0;
								for (a = 0; a < 6; a++) {
									//	printf("核心編號:%d, [ ", hold[a] + 1);
									checkDup[dupCount] = core[hold[a]].skillOrder[2];
									dupCount++;

									//	printf("]\n\n");

								}
								if (checkDup[0] != checkDup[1] &&
									checkDup[0] != checkDup[2] &&
									checkDup[0] != checkDup[3] &&
									checkDup[0] != checkDup[4] &&
									checkDup[0] != checkDup[5] &&
									checkDup[1] != checkDup[2] &&
									checkDup[1] != checkDup[3] &&
									checkDup[1] != checkDup[4] &&
									checkDup[1] != checkDup[5] &&
									checkDup[2] != checkDup[3] &&
									checkDup[2] != checkDup[4] &&
									checkDup[2] != checkDup[5] &&
									checkDup[3] != checkDup[4] &&
									checkDup[3] != checkDup[5] &&
									checkDup[4] != checkDup[5]
									) {
									int prefered = 0;
									/*compare priority and checkdup*/

									for (a = 0; a < 6; a++)
										for (b = 0; b < 6; b++)
											if (priority[a] == checkDup[b])
												prefered++;
									if (prefered == 6)
										printf("\n找到一組6核8技!，且它的主詞符合您持有數量前四多的主詞!");
									/*else
										printf("\n找到一組六核九技!，但它的主詞不符合您持有數量前四多的主詞");
									printf("[%d,%d,%d,", priority[0], priority[1], priority[2]);
									printf("%d,%d,%d]", priority[3], priority[4], priority[5]);*/
									printf("\n");
									if (prefered != 6 && enumerate == '2') {
										for (a = 1; a <= 9; a++) {
											if (core[n].skills[a])
												selected[a] -= 1;
										}
										continue;
									}
									for (a = 0; a < 6; a++) {
										printf("核心編號:%d, [ ", hold[a] + 1);

										for (b = 0; b < 3; b++) {

											printf("%d ", core[hold[a]].skillOrder[b]);


										}
										printf("]\n");

									}

									if (enumerate == '2'&&prefered == 6) { return; }
									else if (enumerate == '3') { return; }
								}
							}
							for (a = 1; a <= 9; a++) {/*unselect 6th core*/
								if (core[n].skills[a])
									selected[a] -= 1;
							}
						}
						for (a = 1; a <= 9; a++) {//unselect 5th core
							if (core[m].skills[a])
								selected[a] -= 1;
						}
					}
					for (a = 1; a <= 9; a++) {/*unselect 4th core*/
						if (core[l].skills[a])
							selected[a] -= 1;
					}
				}
				for (a = 1; a <= 9; a++) {/*unselect 3rd core*/
					if (core[k].skills[a])
						selected[a] -= 1;
				}
				
			}
			for (a = 1; a <= 9; a++) {/*unselect 2nd core*/
				if (core[j].skills[a])
					selected[a] -= 1;
			}
		}
		for (a = 1; a <= 9; a++) {/*unselect 1st core*/
			if (core[i].skills[a])
				selected[a] -= 1;
		}
	}
	printf("\ndone(若都沒有出現\"找到一組6核9技!\"字樣，表示沒有發現可能的組合，請再多蒐集一些核心!)\n");


}