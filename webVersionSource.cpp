#include <stdio.h>
#include <string.h>


struct Core{
	int skills[10];//1 to 9,skills[n]= true if core contains skill n
	int skillOrder[3];//records the ordering of skills
};
struct SkillOccur {
	int skill//which skill
		,occur;//the amount of subjects of this skill
};
void modeOne(struct Core core[], int coreCount, int enumerate,int priority[6]);
void modeTwo(struct Core core[], int coreCount, int enumerate, int priority[6]);
int main() {
	/*參數設定區*/
	char mode = '2';/*設定'1'找4核6技 ,'2'找 6核9技*/
	char enumerate = '2';/*設 '1' 列出所有解,設'2' 列第一組合乎成本解,設'3'列第一組解 */
	printf("歡迎使用楓之谷完美核心篩選器~~\n\n");
	/*請在陣列ｓ1中輸入您持有的核心，用來找四核六技，以','分隔*/
	char s1[2000] = "123,145,456,124,245,236,124,235,126,135,234,136,346,236,356,134,345";
	/*請在陣列ｓ2中輸入您持有的核心，用來找六核九技，以','分隔*/
	char s2[2000] = 
		 "003,003,003,003,003,003,003,007,008,187,128,219,196,159,834,845,837,823,849,845,836,876,897,823,892,819,895,823,871,827,871,831,892,942,947,961,983,914,974,921,978,947,951,964,983,931,942,958,913,654,685,647,675,671,679,684,698,647,629,647,687,361,317,316,368,386,378,386,328,312,394,359,321,394,387,586,549,546,564,583,518,584,538,539,532,518,572,726,768,719,786,764,783,764,746,783,762,243,257,291,476,426,459,431,468,485,459,462,496,473,439,489,429,436,497";
	char *s;
	if (mode == '1')s=s1;
	else s=s2;
	//967841
	struct SkillOccur stack[10];
	int a, b;
	for (a = 0; a < 10; a++) { stack[a].occur = 0; stack[a].skill = a; }
	struct Core core[1000];
	int coreCount = 0;
	char*p=s;
	while (1) {
		
		if (*p == 0)break;
		if (*p == '0' || *(p + 1) == '0') {
			stack[*(p + 2) - 48].occur++;
			p += 4; continue;
		}
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
	b = 9;//the top of the sorted stack
	for (a = 1; a <= b; a++) printf("主詞%d出現%d\n",stack[a].skill,stack[a].occur);
	int priority[6];
	if (mode == '1')
		for (a = 0; a < 4; a++)//we want the top 4 of the stack as priority
			priority[a] = stack[b - a].skill;
	else for (a = 0; a <6; a++)//we want the top 6 of the stack as priority
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
							if (selected[1] == 2/*final check*/
								&& selected[2] == 2
								&& selected[3] == 2
								&& selected[4] == 2
								&& selected[5] == 2
								&& selected[6] == 2
								&& selected[7] == 2
								&& selected[8] == 2
								&& selected[9] == 2
								) {
								//printf("\n找到一組6核9技!\n");
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
										printf("\n找到一組六核九技!，且它的主詞符合您持有數量前六多的主詞!\n");
									/*else
										printf("\n找到一組六核九技!，但它的主詞不符合您持有數量前四多的主詞");
									printf("[%d,%d,%d,", priority[0], priority[1], priority[2]);
									printf("%d,%d,%d]", priority[3], priority[4], priority[5]);*/
									
									if (prefered != 6 && enumerate == '2') {/*unselect 6th core*/ 
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