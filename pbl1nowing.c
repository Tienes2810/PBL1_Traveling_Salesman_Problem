#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define MAX 20
#define MASK(i) (1LL << (i))
#define BIT(x, i) ((x) & MASK(i))
#define SET_ON(x, i) ((x) | MASK(i))
#define SET_OFF(x, i) ((x) & ~MASK(i))
#define INF LLONG_MAX

int c[MAX][MAX], n, X[MAX], visited[MAX], path[MAX];
int d, ans = INT_MAX, cmin = INT_MAX;

void draw_top() {
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
}

void draw_under() {
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
}

void header() {
    draw_top();
    printf("║                                                                    ║\n");
    printf("║           PBL1: DU AN LAP TRINH TINH TOAN                          ║\n");
    printf("║           DE TAI: BAI TOAN NGUOI DU LICH                           ║\n");
    printf("║           Nguoi huong dan: PGS. TS. NGUYEN VAN HIEU                ║\n");
    printf("║           Sinh vien thuc hien:                                     ║\n");
    printf("║           Nguyen Tien           LOP: 24T_Nhat1 NHOM: 13            ║\n");
    printf("║           Tran Minh Truong      LOP: 24T_Nhat1 NHOM: 13            ║\n");
    printf("║                                                                    ║\n");
    draw_under();
}

void input_hand() {
    printf("\n╔════════════════════════════════════╗\n");
    printf("║           NHAP DU LIEU              ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("Nhap so thanh pho n: ");
    if (scanf("%d", &n) != 1 || n < 3 || n > MAX) {
        printf("So thanh pho khong hop le!\n");
        exit(1);
    }
 
    printf("Nhap ma tran chi phi %d x %d:\n", n, n);
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        path[i] = 0;
        for (int j = 0; j < n; j++) {
            if (scanf("%d", &c[i][j]) != 1) {
                printf("Loi nhap du lieu!\n");
                exit(1);
            }
            if (i != j && c[i][j] > 0 && c[i][j] < cmin) cmin = c[i][j];
        }
    }
    if (cmin == INT_MAX) cmin = 1;
  }

void input_file(const char *name_file) {
    FILE *f = fopen(name_file, "r");
    if (!f) {
        printf("Loi mo file %s!\n", name_file);
        exit(1);
    }

    if (fscanf(f, "%d", &n) != 1 || n < 3 || n > MAX) {
        printf("So thanh pho khong hop le!(n > 3)\n");
        fclose(f);
        exit(1);
    }

    printf("\nMa tran chi phi %d x %d:\n", n, n);
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        path[i] = 0;
        for (int j = 0; j < n; j++) {
            if (fscanf(f, "%d", &c[i][j]) != 1 || c[i][j] < 0) {
                printf("Chi phi khong hop le tai [%d][%d]!\n", i, j);
                fclose(f);
                exit(1);
            }
            printf("%d ", c[i][j]);
            if (i != j && c[i][j] > 0 && c[i][j] < cmin) cmin = c[i][j];
        }
        printf("\n");
    }
    if (cmin == INT_MAX) cmin = 1;
    fclose(f);
}

void output_file(const char *name_file) {
    FILE *f = fopen(name_file, "w");
    if (!f) {
        printf("Khong the mo file de ghi!\n");
        exit(1);
    }

    fprintf(f, "So thanh pho: %d\n", n);
    fprintf(f, "Ma tran chi phi:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(f, "%d ", c[i][j]);
        }
        fprintf(f, "\n");
    }

    if (ans == INT_MAX) {
        fprintf(f, "Khong tim duoc duong di!\n");
    } else {
        fprintf(f, "Chi phi nho nhat: %d\n", ans);
        fprintf(f, "Duong di ngan nhat: 1 -> ");
        for (int i = 1; i < n; i++) {
            fprintf(f, "%d -> ", path[i] + 1);
        }
        fprintf(f, "1\n");
    }

    fclose(f);
    printf("\nDa ghi ket qua vao file: %s\n", name_file);
}

void choice_input() {
    printf("\n╔══════════════════════════════╗\n");
    printf("║        NHAP DU LIEU          ║\n");
    printf("╚══════════════════════════════╝\n");
    char name_file[100];
    int a;
    printf("Nhap du lieu:\n1. Tu ban phim\n2. Tu file\nChon: ");
    if (scanf("%d", &a) != 1) {
        printf("Lua chon khong hop le!\n");
        exit(1);
    }

    if (a == 1) {
        input_hand();
    } else if (a == 2) {
        printf("\nHay chon file:\n");
        for (int i = 1; i <= 6; i++) {
            printf("%d. in_put%d.txt\n", i, i);
        }
        int b;
        printf("Chon: ");
        if (scanf("%d", &b) != 1 || b < 1 || b > 6) {
            printf("Lua chon khong hop le!\n");
            exit(1);
        }
        snprintf(name_file, sizeof(name_file), "in_put%d.txt", b);
        input_file(name_file);
    } else {
        printf("Lua chon khong hop le!\n");
        exit(1);
    }
}

void Try(int i, bool outfile) {
    for (int j = 0; j < n; j++) {
        if (!visited[j] && c[X[i - 1]][j] != 0) {
            visited[j] = 1;
            X[i] = j;
            d += c[X[i - 1]][j];

            if (i == n - 1) {
                if (c[X[n - 1]][X[0]] != 0) {
                    int sum = d + c[X[n - 1]][X[0]];
                    if (!outfile) {
                        printf("Duong di: ");
                        for (int k = 0; k < n; k++) {
                            printf("%d -> ", X[k] + 1);
                        }
                        printf("1 (Chi phi: %d)\n", sum);
                    }
                    if (sum < ans) {
                        ans = sum;
                        for (int k = 0; k < n; k++) {
                            path[k] = X[k];
                        }
                    }
                }
            } else if (d + (n - i) * cmin < ans) {
                Try(i + 1, outfile);
            }

            visited[j] = 0;
            d -= c[X[i - 1]][j];
        }
    }
}

void b_and_b(bool outfile) {
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        X[i] = 0;
    }
    X[0] = 0;
    visited[0] = 1;
    d = 0;
    ans = INT_MAX;

    if (!outfile) {
        printf("\nTat ca duong di hop le:\n");
    }
    Try(1, outfile);
    if (ans == INT_MAX) {
        if (!outfile) {
            printf("Khong tim duoc duong di!\n");
        }
        return;
    }
    if (!outfile) {
        printf("\nChi phi nho nhat: %d\nDuong di ngan nhat: 1 -> ", ans);
        for (int i = 1; i < n; i++) printf("%d -> ", path[i] + 1);
        printf("1\n");
    }
}

void free_memory(long long** f, int** parent, int n) {
    if (!f || !parent) return;
    for (int i = 0; i < n; i++) {
        if (f[i]) free(f[i]);
        if (parent[i]) free(parent[i]);
    }
    free(f);
    free(parent);
}

void dynamic_program(bool outfile) {
    if (n > MAX) {
        if (!outfile) {
            printf("So thanh pho qua lon (%d > %d)!\n", n, MAX);
        }
        ans = INT_MAX;
        return;
    }

    ans = INT_MAX;

    long long** f = (long long**)malloc(n * sizeof(long long*));
    int** parent = (int**)malloc(n * sizeof(int*));
    if (!f || !parent) {
        if (!outfile) {
            perror("Loi cap phat bo nho!\n");
        }
        free_memory(f, parent, n);
        ans = INT_MAX;
        return;
    }

    int num_masks = MASK(n);
    for (int i = 0; i < n; i++) {
        f[i] = (long long*)malloc(num_masks * sizeof(long long));
        parent[i] = (int*)malloc(num_masks * sizeof(int));
        if (!f[i] || !parent[i]) {
            if (!outfile) {
                perror("Loi cap phat bo nho!\n");
            }
            free_memory(f, parent, n);
            ans = INT_MAX;
            return;
        }
        for (int j = 0; j < num_masks; j++) {
            f[i][j] = INF;
            parent[i][j] = -1;
        }
    }

    f[0][MASK(0)] = 0;

    for (int mask = 1; mask < num_masks; mask++) {
        for (int u = 0; u < n; u++) {
            if (!BIT(mask, u)) continue;
            for (int v = 0; v < n; v++) {
                if (BIT(mask, v)) continue;
                int prev_mask = SET_OFF(mask, u);
                if (f[u][mask] == INF) continue;
                long long new_cost = f[u][mask] + c[u][v];
                int next_mask = SET_ON(mask, v);
                if (new_cost < f[v][next_mask]) {
                    f[v][next_mask] = new_cost;
                    parent[v][next_mask] = u;
                }
            }
        }
    }

    long long min_cost = INF;
    int dest = -1;
    int final_mask = num_masks - 1;

    for (int i = 0; i < n; i++) {
        if (f[i][final_mask] != INF && c[i][0] > 0) {
            long long new_cost = f[i][final_mask] + c[i][0];
            if (new_cost < min_cost) {
                min_cost = new_cost;
                dest = i;
            }
        }
    }

    if (min_cost == INF) {
        if (!outfile) {
            printf("Khong tim thay duong di\n");
        }
        ans = INT_MAX;
    } else {
        ans = (int)min_cost;
        if (!outfile) {
            printf("\nChi phi nho nhat: %d\n", ans);
            printf("Duong di ngan nhat: ");
        }

        int path_qhd[MAX], path_count = 0;
        int current_mask = final_mask;
        while (dest != -1) {
            path_qhd[path_count++] = dest;
            int prev = parent[dest][current_mask];
            current_mask = SET_OFF(current_mask, dest);
            dest = prev;
        }

        for (int i = 0; i < n; i++) path[i] = 0;
        path[0] = 0;
        for (int i = path_count - 2, k = 1; i >= 0; i--, k++) {
            path[k] = path_qhd[i];
        }

        if (!outfile) {
            printf("1 -> ");
            for (int i = n-1; i >= 1; i--) {
                printf("%d -> ", path[i] + 1);
            }
            printf("1\n");
        }
    }

    free_memory(f, parent, n);
}

void choice_method_and_ouput() {
    char name_file[100];
    int a, b;
    printf("\n╔══════════════════════════════╗\n");
    printf("║      LUA CHON THUAT TOAN     ║\n");
    printf("╚══════════════════════════════╝\n");
    printf("So thanh pho la: %d\n", n);
    if (n <= 7) {
        printf("1. Dung Nhanh can (toi uu hon).\n");
        printf("2. Dung Quy Hoach Dong.\n");
    } else {
        printf("1. Dung Nhanh can.\n");
        printf("2. Dung Quy Hoach Dong (toi uu hon).\n");
    }
    printf("Nhap lua chon: ");

    if (scanf("%d", &a) != 1 || (a != 1 && a != 2)) {
        printf("Lua chon khong hop le!\n");
        exit(1);
    }
     printf("\n╔══════════════════════════════╗\n");
    printf("║     LUA CHON XUAT KET QUA    ║\n");
    printf("╚══════════════════════════════╝\n");
    printf("1. Xuat ra man hinh\n2. Xuat vao File\nChon: ");

    if (scanf("%d", &b) != 1 || (b != 1 && b != 2)) {
        printf("Lua chon khong hop le!\n");
        exit(1);
    }

    bool outfile = (b == 2);

    if (a == 1) {
        b_and_b(outfile);
    } else {
        dynamic_program(outfile);
    }

    if (b == 2) {
        printf("\nHay chon file:\n");
        for (int i = 1; i <= 6; i++) {
            printf("%d. out_put%d.txt\n", i, i);
        }
        int c;
        printf("Chon: ");
        if (scanf("%d", &c) != 1 || c < 1 || c > 6) {
            printf("Lua chon khong hop le!\n");
            exit(1);
        }
        snprintf(name_file, sizeof(name_file), "out_put%d.txt", c);
        output_file(name_file);
    }
}

int main() {
    header();
    choice_input();
    choice_method_and_ouput();
    return 0;
}