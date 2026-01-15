#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
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

void input_file(const char *name_file, char *statusMessage) {
    FILE *f = fopen(name_file, "r");
    if (!f) {
        snprintf(statusMessage, 150, "Lỗi: Không thể mở file %s!", name_file);
        n = 0;
        return;
    }

    if (fscanf(f, "%d", &n) != 1 || n < 3 || n > MAX) {
        snprintf(statusMessage, 150, "Lỗi: Số thành phố không hợp lệ (3 <= n <= %d)!", MAX);
        fclose(f);
        n = 0;
        return;
    }

    for (int i = 0; i < n; i++) {
        visited[i] = path[i] = 0;
        for (int j = 0; j < n; j++) {
            if (fscanf(f, "%d", &c[i][j]) != 1) {
                snprintf(statusMessage, 150, "Lỗi: Chi phí không hợp lệ tại [%d][%d] trong %s!", i, j, name_file);
                fclose(f);
                n = 0;
                return;
            }
            if (i != j && c[i][j] > 0 && c[i][j] < cmin) cmin = c[i][j];
        }
    }
    if (cmin == INT_MAX && n > 0) cmin = 1;
    fclose(f);
     if (n > 0 && strlen(statusMessage) == 0) {
        snprintf(statusMessage, 150, "Đã tải thành công file %s.", name_file);
    }
}

void output_file(const char *name_file) {
    FILE *f = fopen(name_file, "w");
    if (!f) {
        char errorMsg[100];
        snprintf(errorMsg, sizeof(errorMsg), "Lỗi: Không thể mở file %s để ghi!", name_file);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Lỗi File", errorMsg, NULL);
        return;
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
    char successMsg[100];
    snprintf(successMsg, sizeof(successMsg), "Đã ghi kết quả vào file: %s", name_file);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hoàn Thành", successMsg, NULL);
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
                    if (sum < ans) {
                        ans = sum;
                        for (int k = 0; k < n; k++) {
                            path[k] = X[k];
                        }
                    }
                }
            } else if (d + (n - (i + 1) + 1) * cmin < ans) {
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
    Try(1, outfile);
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
        ans = INT_MAX;
        return;
    }

    ans = INT_MAX;
    long long** f = (long long**)malloc(n * sizeof(long long*));
    int** parent = (int**)malloc(n * sizeof(int*));
    if (!f || !parent) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Lỗi Bộ Nhớ", "Không đủ bộ nhớ để chạy thuật toán!", NULL);
        free_memory(f, parent, n);
         ans = INT_MAX;
        return;
    }

    int num_masks = MASK(n);
    for (int i = 0; i < n; i++) {
        f[i] = (long long*)malloc(num_masks * sizeof(long long));
        parent[i] = (int*)malloc(num_masks * sizeof(int));
        if (!f[i] || !parent[i]) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Lỗi Bộ Nhớ", "Không đủ bộ nhớ để chạy thuật toán!", NULL);
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

    long long min_cost= INF;
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
        ans = INT_MAX;
    } else {
        ans = (int)min_cost;

        int path_dp[MAX], path_count = 0;
        int current_mask = final_mask;

        while (dest != -1 && path_count < n) {
            path_dp[path_count++] = dest;
            int prev = parent[dest][current_mask];
            current_mask = SET_OFF(current_mask, dest);
            dest = prev;
        }

        for (int i = 0; i < n; i++) path[i] = 0;
        path[0] = 0;
        for (int i = path_count - 2, k = 1; i >= 0; i--, k++) {
            path[k] = path_dp[i];
        }
    }
    free_memory(f, parent, n);
}

void SDL_FakeMouseButtonEvent(SDL_Rect* button, Uint8 button_type); 

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

typedef enum { TITLE, MENU, INPUT_FILES, OUTPUT_FILES, RESULT, MANUAL_INPUT } ScreenState;

typedef struct {
    int input_type;
    int input_file_choice;
    int method;
    int output_type;
    int output_file_choice;
    bool run_triggered;
    bool manual_input_done;
    bool show_graph;
} MenuChoices;

typedef struct {
    char buffer[512];
    int buffer_len;
    char error_msg[150];
    int scroll_offset_y;
    int total_text_height;
    int scroll_offset_x; 
} ManualInputState;

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color, int targetWidthForCentering, bool centerInTarget) {
    if (!text || strlen(text) == 0) return;
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dest = {x, y, w, h};
    if (centerInTarget && targetWidthForCentering > 0) {
        dest.x = x + (targetWidthForCentering - w) / 2;
    } else if (centerInTarget) {
        dest.x = (SCREEN_WIDTH - w) / 2;
    }
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void getTextDimensions(TTF_Font* font, const char* text, SDL_Color color, int* w, int* h) {
    if (!text || !font) {
        if (w) *w = 0;
        if (h) *h = 0;
        return;
    }
    if (TTF_SizeUTF8(font, text, w, h) != 0) {
        if (w) *w = 0;
        if (h) *h = 0;
    }
}

bool point_in_rect(int x, int y, SDL_Rect* rect) {
    return (x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h);
}

void drawArrow(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color, int headSize) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

    double angle = atan2(y2 - y1, x2 - x1);
    int arrowLength = headSize;
    double arrowAngle = M_PI / 5;

    int ax1 = x2 - (int)(arrowLength * cos(angle - arrowAngle));
    int ay1 = y2 - (int)(arrowLength * sin(angle - arrowAngle));
    SDL_RenderDrawLine(renderer, x2, y2, ax1, ay1);

    int ax2 = x2 - (int)(arrowLength * cos(angle + arrowAngle));
    int ay2 = y2 - (int)(arrowLength * sin(angle + arrowAngle));
    SDL_RenderDrawLine(renderer, x2, y2, ax2, ay2);
}

void renderFilledCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int y_offset = -radius; y_offset <= radius; y_offset++) {
        int x_offset_max = (int)sqrt((double)radius * radius - (double)y_offset * y_offset);
        SDL_RenderDrawLine(renderer, centerX - x_offset_max, centerY + y_offset,
                                     centerX + x_offset_max, centerY + y_offset);
    }
}

void renderCircleOutline(SDL_Renderer *renderer, int centerX, int centerY, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int x = radius - 1;
    if (radius == 0) {
        SDL_RenderDrawPoint(renderer, centerX, centerY);
        return;
    }
    if (radius < 0) radius = -radius;

    int y = 0;
    int dx_err = 1;
    int dy_err = 1;
    int err = dx_err - (radius << 1);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);

        if (err <= 0) {
            y++;
            err += dy_err;
            dy_err += 2;
        }
        if (err > 0) {
            x--;
            dx_err += 2;
            err += dx_err - (radius << 1);
        }
    }
}

void renderGraph(SDL_Renderer* renderer, TTF_Font* font, SDL_Color nodeColor, SDL_Color edgeColor, SDL_Color labelColor, SDL_Color costColor, int centerX, int centerY, int radius) {
    if (n == 0 || ans == INT_MAX) return;

    SDL_Point nodes_display_pos[MAX];
    const int nodeDrawRadius = 15;
    const int arrowHeadSize = 12;

    for (int i = 0; i < n; i++) {
        float angle = 2 * M_PI * i / n;
        nodes_display_pos[i].x = centerX + (int)(radius * cos(angle));
        nodes_display_pos[i].y = centerY + (int)(radius * sin(angle));
    }

    for (int i = 0; i < n; i++) {
        int u_idx = path[i];
        int v_idx = path[(i + 1) % n];

        SDL_Point p1_center = nodes_display_pos[u_idx];
        SDL_Point p2_center = nodes_display_pos[v_idx];

        double dir_x = p2_center.x - p1_center.x;
        double dir_y = p2_center.y - p1_center.y;
        double dist = sqrt(dir_x * dir_x + dir_y * dir_y);

        if (dist == 0) continue;

        double norm_dir_x = dir_x / dist;
        double norm_dir_y = dir_y / dist;

        int line_start_x = p1_center.x + (int)(norm_dir_x * nodeDrawRadius);
        int line_start_y = p1_center.y + (int)(norm_dir_y * nodeDrawRadius);
        int line_end_x = p2_center.x - (int)(norm_dir_x * nodeDrawRadius);
        int line_end_y = p2_center.y - (int)(norm_dir_y * nodeDrawRadius);

        drawArrow(renderer, line_start_x, line_start_y, line_end_x, line_end_y, edgeColor, arrowHeadSize);

        char cost_text_str[10];
        snprintf(cost_text_str, sizeof(cost_text_str), "%d", c[u_idx][v_idx]);
        int textW, textH;
        getTextDimensions(font, cost_text_str, costColor, &textW, &textH);

        double midX_orig = p1_center.x + dir_x / 2.0;
        double midY_orig = p1_center.y + dir_y / 2.0;
        double text_offset_distance = textH * 0.7 + 10;
        double norm_perp_x = -norm_dir_y;
        double norm_perp_y = norm_dir_x;

        int cost_text_render_x = (int)(midX_orig + norm_perp_x * text_offset_distance - textW / 2.0);
        int cost_text_render_y = (int)(midY_orig + norm_perp_y * text_offset_distance - textH / 2.0);

        SDL_Rect text_bg_rect = {cost_text_render_x - 3, cost_text_render_y - 2, textW + 6, textH + 4};
        SDL_Color white_bg = {255, 255, 255, 200};

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, white_bg.r, white_bg.g, white_bg.b, white_bg.a);
        SDL_RenderFillRect(renderer, &text_bg_rect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        renderText(renderer, font, cost_text_str, cost_text_render_x, cost_text_render_y, costColor, 0, false);
    }

    SDL_Color borderColor = {0, 0, 0, 255};
    for (int i = 0; i < n; i++) {
        renderFilledCircle(renderer, nodes_display_pos[i].x, nodes_display_pos[i].y, nodeDrawRadius, nodeColor);
        renderCircleOutline(renderer, nodes_display_pos[i].x, nodes_display_pos[i].y, nodeDrawRadius, borderColor);

        char label_text_node[10];
        snprintf(label_text_node, sizeof(label_text_node), "%d", i + 1);
        int textW_node, textH_node;

        SDL_Color nodeLabelColor = {255, 255, 255, 255};
        if (nodeColor.r * 0.299 + nodeColor.g * 0.587 + nodeColor.b * 0.114 > 186) {
             nodeLabelColor = (SDL_Color){0,0,0,255};
        }

        getTextDimensions(font, label_text_node, nodeLabelColor, &textW_node, &textH_node);
        renderText(renderer, font, label_text_node, nodes_display_pos[i].x - textW_node / 2, nodes_display_pos[i].y - textH_node / 2, nodeLabelColor, 0, false);
    }
}

void calculate_cursor_and_total_height(TTF_Font* font, const char* text_buffer, int buffer_len,
                                       int input_box_content_start_x, int input_box_content_start_y, int line_h,
                                       int* out_cursor_canvas_x, int* out_cursor_canvas_y, int* out_total_text_height) {
    *out_cursor_canvas_x = input_box_content_start_x;
    *out_cursor_canvas_y = input_box_content_start_y;
    *out_total_text_height = line_h;

    for (int k = 0; k < buffer_len; ++k) {
        if (text_buffer[k] == '\n') {
            *out_cursor_canvas_y += line_h;
            *out_cursor_canvas_x = input_box_content_start_x;
            *out_total_text_height += line_h;
        } else {
            char single_char_str[2] = {text_buffer[k], '\0'};
            int char_w, char_h_dummy;
            getTextDimensions(font, single_char_str, (SDL_Color){0,0,0,255}, &char_w, &char_h_dummy);
            *out_cursor_canvas_x += char_w;
        }
    }
}

void update_manual_input_scroll_after_edit(ManualInputState* mi_state, TTF_Font* font, const SDL_Rect* p_inputBox) {
    int line_h = TTF_FontHeight(font) - 2;
    int text_area_top_padding = 5;
    int text_area_left_padding = 5;
    int visible_text_area_height = p_inputBox->h - text_area_top_padding * 2;
    int visible_text_area_width = p_inputBox->w - text_area_left_padding * 2;

    int cursor_x_on_canvas, cursor_y_on_canvas_line_start;
    int current_total_text_height;

    calculate_cursor_and_total_height(font, mi_state->buffer, mi_state->buffer_len, p_inputBox->x + text_area_left_padding, p_inputBox->y + text_area_top_padding, line_h, &cursor_x_on_canvas, &cursor_y_on_canvas_line_start, &current_total_text_height);
    mi_state->total_text_height = current_total_text_height;

    if (cursor_y_on_canvas_line_start + line_h > (p_inputBox->y + text_area_top_padding + visible_text_area_height) + mi_state->scroll_offset_y) {
        mi_state->scroll_offset_y = cursor_y_on_canvas_line_start + line_h - (p_inputBox->y + text_area_top_padding + visible_text_area_height);
    }
    if (cursor_y_on_canvas_line_start < (p_inputBox->y + text_area_top_padding) + mi_state->scroll_offset_y) {
         mi_state->scroll_offset_y = cursor_y_on_canvas_line_start - (p_inputBox->y + text_area_top_padding);
    }

    int max_scroll_y = mi_state->total_text_height - visible_text_area_height;
    if (max_scroll_y < 0) max_scroll_y = 0;
    if (mi_state->scroll_offset_y > max_scroll_y) mi_state->scroll_offset_y = max_scroll_y;
    if (mi_state->scroll_offset_y < 0) mi_state->scroll_offset_y = 0;

    const int CURSOR_SAFETY_MARGIN_X = 10;
    int text_area_screen_start_x = p_inputBox->x + text_area_left_padding;

    if (cursor_x_on_canvas > text_area_screen_start_x + mi_state->scroll_offset_x + visible_text_area_width - CURSOR_SAFETY_MARGIN_X) {
        mi_state->scroll_offset_x = cursor_x_on_canvas - (text_area_screen_start_x + visible_text_area_width - CURSOR_SAFETY_MARGIN_X);
    }

    if (cursor_x_on_canvas < text_area_screen_start_x + mi_state->scroll_offset_x) {
        mi_state->scroll_offset_x = cursor_x_on_canvas - text_area_screen_start_x;
    }

    if (mi_state->scroll_offset_x < 0) {
        mi_state->scroll_offset_x = 0;
    }
}


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Lỗi SDL", "Không thể khởi tạo SDL!", NULL); return 1; }
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Lỗi SDL_image", "Không thể khởi tạo SDL_image!", NULL); SDL_Quit(); return 1; }
    if (TTF_Init() == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Lỗi TTF", "Không thể khởi tạo SDL_ttf!", NULL); IMG_Quit(); SDL_Quit(); return 1; }

    SDL_Window* window = SDL_CreateWindow("Bài Toán Người Du Lịch - PBL1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font* fontTitle = TTF_OpenFont("Arial.ttf", 32);
    TTF_Font* fontBold = TTF_OpenFont("Arial.ttf", 26);
    TTF_Font* fontNormal = TTF_OpenFont("Arial.ttf", 22);
    TTF_Font* fontSmall = TTF_OpenFont("Arial.ttf", 18);
    if (!fontTitle || !fontBold || !fontNormal || !fontSmall) {

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Lỗi Font", "Không thể tải font Arial.ttf!", NULL);
        if (fontTitle) TTF_CloseFont(fontTitle); if (fontBold) TTF_CloseFont(fontBold);
        if (fontNormal) TTF_CloseFont(fontNormal); if (fontSmall) TTF_CloseFont(fontSmall);
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); TTF_Quit(); IMG_Quit(); SDL_Quit(); return 1;
    }
    TTF_SetFontStyle(fontBold, TTF_STYLE_BOLD);

    SDL_Texture* logoTexture = NULL;
    SDL_Surface* logoSurface = IMG_Load("logo.jpeg"); if (!logoSurface) logoSurface = IMG_Load("logo.png");
    if (!logoSurface) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Cảnh báo Hình Ảnh", "Không thể tải logo.jpeg hoặc logo.png.", NULL);
    else { logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface); SDL_FreeSurface(logoSurface); }

    SDL_Color black = {0,0,0,255}, white={255,255,255,255}, gray={200,200,200,255}, dark_gray={150,150,150,255}, red_color={255,0,0,255}, blue_color={0,0,255,255}, green_cost={0,100,0,255};

    bool quit = false;
    ScreenState screen = TITLE;
    MenuChoices menu = {0,0,0,0,0,false,false,true};
    ManualInputState manual_input = {{0},0,{0},0,0,0}; 
    char statusMessage[150] = "";
    SDL_Event e;

    SDL_Rect startButton = {(SCREEN_WIDTH - 200) / 2, 490, 200, 50};
    int menuLabelY = 80;
    int menuButtonY_input = menuLabelY + 40;
    int menuButtonY_method = menuButtonY_input + 60 + 20;
    int menuButtonY_output = menuButtonY_method + 60 + 20;
    int menuButtonY_graph = menuButtonY_output + 60 + 20;
    int menuButtonY_controls = menuButtonY_graph + 60 + 20;
    int menuButtonWidth = 250;
    int menuButtonHeight = 50;
    int menuButtonSpacingX = 40;
    int menuTotalButtonWidthForTwo = 2 * menuButtonWidth + menuButtonSpacingX;
    int menuStartX_Pair = (SCREEN_WIDTH - menuTotalButtonWidthForTwo) / 2;
    SDL_Rect inputHandButton = {menuStartX_Pair, menuButtonY_input, menuButtonWidth, menuButtonHeight};
    SDL_Rect inputFileButton = {menuStartX_Pair + menuButtonWidth + menuButtonSpacingX, menuButtonY_input, menuButtonWidth, menuButtonHeight};
    SDL_Rect methodBBButton = {menuStartX_Pair, menuButtonY_method, menuButtonWidth, menuButtonHeight};
    SDL_Rect methodDPButton = {menuStartX_Pair + menuButtonWidth + menuButtonSpacingX, menuButtonY_method, menuButtonWidth, menuButtonHeight};
    SDL_Rect outputScreenButton = {menuStartX_Pair, menuButtonY_output, menuButtonWidth, menuButtonHeight};
    SDL_Rect outputFileButton = {menuStartX_Pair + menuButtonWidth + menuButtonSpacingX, menuButtonY_output, menuButtonWidth, menuButtonHeight};
    SDL_Rect showGraphButton = {menuStartX_Pair, menuButtonY_graph, menuButtonWidth, menuButtonHeight};
    SDL_Rect hideGraphButton = {menuStartX_Pair + menuButtonWidth + menuButtonSpacingX, menuButtonY_graph, menuButtonWidth, menuButtonHeight};
    SDL_Rect runButton = {(SCREEN_WIDTH - menuButtonWidth) / 2, menuButtonY_controls, menuButtonWidth, menuButtonHeight};
    SDL_Rect backButton = {(SCREEN_WIDTH - 200) / 2, SCREEN_HEIGHT - 70, 200, 50};
    SDL_Rect fileButtons[6];
    int fileButtonGridX = 150; int fileButtonGridY = 150; int fileButtonW = 150; int fileButtonH = 50;
    int fileButtonMarginX = 25; int fileButtonMarginY = 20;
    for (int i = 0; i < 6; i++) {
        fileButtons[i].x = fileButtonGridX + (i % 3) * (fileButtonW + fileButtonMarginX);
        fileButtons[i].y = fileButtonGridY + (i / 3) * (fileButtonH + fileButtonMarginY);
        fileButtons[i].w = fileButtonW; fileButtons[i].h = fileButtonH;
    }
    SDL_Rect backButtonFileSelection = {(SCREEN_WIDTH - 200)/2, fileButtonGridY + 2 * (fileButtonH + fileButtonMarginY) + 30, 200, 50};
    SDL_Rect submitButton = {(SCREEN_WIDTH - 200) / 2, 360, 200, 50};
    SDL_Rect backButtonManual = {(SCREEN_WIDTH - 200) / 2, 420, 200, 50};
    SDL_Rect inputBox = {(SCREEN_WIDTH - 400) / 2, 170, 400, 180};


    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x_mouse = e.button.x; int y_mouse = e.button.y;
                strcpy(statusMessage, ""); strcpy(manual_input.error_msg, "");
                 if (screen == TITLE && point_in_rect(x_mouse, y_mouse, &startButton)) { screen = MENU;
                } else if (screen == MENU) {
                    if (point_in_rect(x_mouse, y_mouse, &inputHandButton)) {
                        menu.input_type = 1; menu.input_file_choice = 0; menu.manual_input_done = false;
                        screen = MANUAL_INPUT;
                        manual_input.buffer[0] = '\0'; manual_input.buffer_len = 0; manual_input.error_msg[0] = '\0';
                        manual_input.scroll_offset_y = 0;
                        manual_input.scroll_offset_x = 0; 
                        manual_input.total_text_height = TTF_FontHeight(fontNormal) -2;
                        SDL_StartTextInput();
                    } else if (point_in_rect(x_mouse, y_mouse, &inputFileButton)) {menu.input_type = 2; menu.manual_input_done = false; screen = INPUT_FILES;
                    } else if (point_in_rect(x_mouse, y_mouse, &methodBBButton)) { menu.method = 1;
                    } else if (point_in_rect(x_mouse, y_mouse, &methodDPButton)) { menu.method = 2;
                    } else if (point_in_rect(x_mouse, y_mouse, &outputScreenButton)) { menu.output_type = 1; menu.output_file_choice = 0;
                    } else if (point_in_rect(x_mouse, y_mouse, &outputFileButton)) { menu.output_type = 2; screen = OUTPUT_FILES;
                    } else if (point_in_rect(x_mouse, y_mouse, &showGraphButton)) { menu.show_graph = true;
                    } else if (point_in_rect(x_mouse, y_mouse, &hideGraphButton)) { menu.show_graph = false;
                    } else if (point_in_rect(x_mouse, y_mouse, &runButton)) { menu.run_triggered = true;
                    } else if (point_in_rect(x_mouse, y_mouse, &backButton)) { screen = TITLE; menu = (MenuChoices){0,0,0,0,0,false,false,true}; }
                } else if (screen == INPUT_FILES) {for (int i = 0; i < 6; i++) { if (point_in_rect(x_mouse, y_mouse, &fileButtons[i])) { menu.input_file_choice = i + 1; menu.manual_input_done = false; screen = MENU; break; } } if (point_in_rect(x_mouse, y_mouse, &backButtonFileSelection)) { screen = MENU; }
                } else if (screen == OUTPUT_FILES) { for (int i = 0; i < 6; i++) { if (point_in_rect(x_mouse, y_mouse, &fileButtons[i])) { menu.output_file_choice = i + 1; screen = MENU; break; } } if (point_in_rect(x_mouse, y_mouse, &backButtonFileSelection)) { screen = MENU; }
                } else if (screen == RESULT && point_in_rect(x_mouse, y_mouse, &backButton)) { screen = MENU;
                } else if (screen == MANUAL_INPUT) {
                    if (point_in_rect(x_mouse, y_mouse, &submitButton)) {
                         if (manual_input.buffer_len == 0) { snprintf(manual_input.error_msg, sizeof(manual_input.error_msg), "Lỗi: Vui lòng nhập dữ liệu!");
                        } else {
                            char temp_buffer_parse[512]; strncpy(temp_buffer_parse, manual_input.buffer, sizeof(temp_buffer_parse) - 1); temp_buffer_parse[sizeof(temp_buffer_parse) - 1] = '\0';
                            char *current_pos_in_buffer = temp_buffer_parse; char *next_line_ptr = NULL;
                            next_line_ptr = strchr(current_pos_in_buffer, '\n'); if (next_line_ptr) { *next_line_ptr = '\0'; }
                            int temp_n_val = atoi(current_pos_in_buffer);
                            if (temp_n_val < 3 || temp_n_val > MAX) { snprintf(manual_input.error_msg, sizeof(manual_input.error_msg), "Lỗi: Số đỉnh (dòng đầu tiên) phải từ 3 đến %d!", MAX); if (next_line_ptr) *next_line_ptr = '\n';
                            } else {
                                if (next_line_ptr) { *next_line_ptr = '\n'; current_pos_in_buffer = next_line_ptr + 1; } else { current_pos_in_buffer += strlen(current_pos_in_buffer); }
                                int expected_count = temp_n_val * temp_n_val; int values[MAX * MAX]; int count = 0; bool parse_error_matrix = false;
                                char *token = strtok(current_pos_in_buffer, " \t\r\n");
                                while (token != NULL && count < expected_count) {
                                    char* endptr; long val_long = strtol(token, &endptr, 10);
                                    if (endptr == token || (*endptr != '\0' && *endptr != ' ' && *endptr != '\t' && *endptr != '\r' && *endptr != '\n') || val_long < 0 || val_long > INT_MAX) {
                                        snprintf(manual_input.error_msg, sizeof(manual_input.error_msg), "Lỗi: Chi phí ma trận '%s' không hợp lệ!", token); parse_error_matrix = true; break;
                                    } values[count++] = (int)val_long; token = strtok(NULL, " \t\r\n");
                                }
                                if (parse_error_matrix) {}
                                else if (count != expected_count) { snprintf(manual_input.error_msg, sizeof(manual_input.error_msg), "Lỗi: Cần đúng %dx%d=%d chi phí. Đã nhập %d.", temp_n_val, temp_n_val, expected_count, count);
                                } else {
                                    n = temp_n_val; cmin = INT_MAX;
                                    for (int r_idx = 0; r_idx < n; r_idx++) { visited[r_idx] = path[r_idx] = 0; for (int col_idx = 0; col_idx < n; col_idx++) { c[r_idx][col_idx] = values[r_idx * n + col_idx]; if (r_idx != col_idx && c[r_idx][col_idx] > 0 && c[r_idx][col_idx] < cmin) { cmin = c[r_idx][col_idx]; } } }
                                    if (cmin == INT_MAX && n > 0) cmin = 1;
                                    menu.manual_input_done = true; SDL_StopTextInput(); screen = MENU; snprintf(statusMessage, sizeof(statusMessage), "Nhập thành công %d đỉnh.", n); strcpy(manual_input.error_msg, "");
                                }
                            }
                        }
                    } else if (point_in_rect(x_mouse, y_mouse, &backButtonManual)) {
                        screen = MENU; SDL_StopTextInput();
                    }
                }

            } else if (screen == MANUAL_INPUT && e.type == SDL_MOUSEWHEEL) {
                int line_h = TTF_FontHeight(fontNormal) - 2;
                int scroll_speed = line_h * 3;
                int text_area_top_padding = 5;
                int visible_text_area_height = inputBox.h - text_area_top_padding * 2;

                if (e.wheel.y > 0) { 
                    manual_input.scroll_offset_y -= scroll_speed;
                    if (manual_input.scroll_offset_y < 0) manual_input.scroll_offset_y = 0;
                } else if (e.wheel.y < 0) { 
                    int max_scroll = manual_input.total_text_height - visible_text_area_height;
                    if (max_scroll < 0) max_scroll = 0;
                    manual_input.scroll_offset_y += scroll_speed;
                    if (manual_input.scroll_offset_y > max_scroll) manual_input.scroll_offset_y = max_scroll;
                }
            } else if (screen == MANUAL_INPUT && e.type == SDL_TEXTINPUT) {
                 if (manual_input.buffer_len + strlen(e.text.text) < sizeof(manual_input.buffer) - 1) {
                    strcat(manual_input.buffer, e.text.text);
                    manual_input.buffer_len += strlen(e.text.text);
                    update_manual_input_scroll_after_edit(&manual_input, fontNormal, &inputBox);
                }
            } else if (screen == MANUAL_INPUT && e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && manual_input.buffer_len > 0) {
                    manual_input.buffer[--manual_input.buffer_len] = '\0';
                    update_manual_input_scroll_after_edit(&manual_input, fontNormal, &inputBox);
                } else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                    if (manual_input.buffer_len < sizeof(manual_input.buffer) - 2) {
                        manual_input.buffer[manual_input.buffer_len++] = '\n';
                        manual_input.buffer[manual_input.buffer_len] = '\0';
                        update_manual_input_scroll_after_edit(&manual_input, fontNormal, &inputBox);
                    }
                }
            }
        } 

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); SDL_RenderClear(renderer);
        SDL_Point mouse_pos; SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        int text_render_w, text_render_h;

        if (screen == TITLE) {
            renderText(renderer, fontNormal, "TRƯỜNG ĐẠI HỌC BÁCH KHOA - ĐẠI HỌC ĐÀ NẴNG", 0, 15, black, SCREEN_WIDTH, true);
            renderText(renderer, fontBold, "KHOA CÔNG NGHỆ THÔNG TIN", 0, 45, black, SCREEN_WIDTH, true);
            if (logoTexture) { int logoW_render, logoH_render; SDL_QueryTexture(logoTexture, NULL, NULL, &logoW_render, &logoH_render); int maxSize = 100; float ratio = (float)logoW_render / logoH_render; if (logoW_render > maxSize || logoH_render > maxSize) { if (ratio > 1) { logoW_render = maxSize; logoH_render = (int)(maxSize / ratio); } else { logoH_render = maxSize; logoW_render = (int)(maxSize * ratio); } } SDL_Rect logoRect = {(SCREEN_WIDTH - logoW_render) / 2, 80, logoW_render, logoH_render}; SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect); }
            renderText(renderer, fontTitle, "ĐỒ ÁN LẬP TRÌNH TÍNH TOÁN", 0, 190, black, SCREEN_WIDTH, true);
            renderText(renderer, fontBold, "ĐỀ TÀI: BÀI TOÁN NGƯỜI DU LỊCH (TSP)", 0, 240, black, SCREEN_WIDTH, true);
            renderText(renderer, fontNormal, "GVHD: PGS. TS. NGUYỄN VĂN HIỆU", 0, 300, black, SCREEN_WIDTH, true);
            renderText(renderer, fontNormal, "Sinh viên thực hiện:", 0, 350, black, SCREEN_WIDTH, true);
            renderText(renderer, fontNormal, "Nguyễn Tiến  - LỚP: 24T_Nhat1 - NHÓM: 13", 0, 390, black, SCREEN_WIDTH, true);
            renderText(renderer, fontNormal, "Trần Minh Trường - LỚP: 24T_Nhat1 - NHÓM: 13", 0, 420, black, SCREEN_WIDTH, true);
            SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &startButton) ? gray.r : white.r, point_in_rect(mouse_pos.x, mouse_pos.y, &startButton) ? gray.g : white.g, point_in_rect(mouse_pos.x, mouse_pos.y, &startButton) ? gray.b : white.b, 255); SDL_RenderFillRect(renderer, &startButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &startButton); getTextDimensions(fontBold, "Bắt đầu", black, &text_render_w, &text_render_h); renderText(renderer, fontBold, "Bắt đầu", startButton.x + (startButton.w - text_render_w)/2, startButton.y + (startButton.h - text_render_h)/2, black, 0, false);
        } else if (screen == MENU) { 
             renderText(renderer, fontTitle, "CÀI ĐẶT THUẬT TOÁN", 0, 30, black, SCREEN_WIDTH, true); if (strlen(statusMessage) > 0) { SDL_Color statusMsgColorRender = (strncmp(statusMessage, "Lỗi", strlen("Lỗi")) == 0 || strncmp(statusMessage, "Vui lòng", strlen("Vui lòng")) == 0 || strncmp(statusMessage, "Không tìm", strlen("Không tìm"))==0) ? red_color : green_cost; getTextDimensions(fontSmall, statusMessage, statusMsgColorRender, &text_render_w, &text_render_h); renderText(renderer, fontSmall, statusMessage, (SCREEN_WIDTH - text_render_w)/2, 60, statusMsgColorRender, 0, false); }
            renderText(renderer, fontBold, "Chọn Input:", menuStartX_Pair, menuButtonY_input - 30, black, 0, false); SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &inputHandButton) ? gray.r : (menu.input_type == 1 ? dark_gray.r : white.r), point_in_rect(mouse_pos.x, mouse_pos.y, &inputHandButton) ? gray.g : (menu.input_type == 1 ? dark_gray.g : white.g), point_in_rect(mouse_pos.x, mouse_pos.y, &inputHandButton) ? gray.b : (menu.input_type == 1 ? dark_gray.b : white.b), 255); SDL_RenderFillRect(renderer, &inputHandButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &inputHandButton); getTextDimensions(fontNormal, "Nhập thủ công", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Nhập thủ công", inputHandButton.x + (inputHandButton.w - text_render_w)/2, inputHandButton.y + (inputHandButton.h - text_render_h)/2, black, 0, false);
            SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &inputFileButton) ? gray.r : (menu.input_type == 2 ? dark_gray.r : white.r), point_in_rect(mouse_pos.x, mouse_pos.y, &inputFileButton) ? gray.g : (menu.input_type == 2 ? dark_gray.g : white.g), point_in_rect(mouse_pos.x, mouse_pos.y, &inputFileButton) ? gray.b : (menu.input_type == 2 ? dark_gray.b : white.b), 255); SDL_RenderFillRect(renderer, &inputFileButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &inputFileButton); char inputFileTextRender[50]; if (menu.input_type == 2 && menu.input_file_choice > 0) snprintf(inputFileTextRender, sizeof(inputFileTextRender), "Input File (%d)", menu.input_file_choice); else strcpy(inputFileTextRender, "Chọn Input File"); getTextDimensions(fontNormal, inputFileTextRender, black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, inputFileTextRender, inputFileButton.x + (inputFileButton.w - text_render_w)/2, inputFileButton.y + (inputFileButton.h - text_render_h)/2, black, 0, false);
            renderText(renderer, fontBold, "Chọn Thuật Toán:", menuStartX_Pair, menuButtonY_method - 30, black, 0, false); SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &methodBBButton) ? gray.r : (menu.method == 1 ? dark_gray.r : white.r), point_in_rect(mouse_pos.x, mouse_pos.y, &methodBBButton) ? gray.g : (menu.method == 1 ? dark_gray.g : white.g), point_in_rect(mouse_pos.x, mouse_pos.y, &methodBBButton) ? gray.b : (menu.method == 1 ? dark_gray.b : white.b), 255); SDL_RenderFillRect(renderer, &methodBBButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &methodBBButton); getTextDimensions(fontNormal, "Nhánh và Cận", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Nhánh và Cận", methodBBButton.x + (methodBBButton.w - text_render_w)/2, methodBBButton.y + (methodBBButton.h - text_render_h)/2, black, 0, false);
            SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &methodDPButton) ? gray.r : (menu.method == 2 ? dark_gray.r : white.r), point_in_rect(mouse_pos.x, mouse_pos.y, &methodDPButton) ? gray.g : (menu.method == 2 ? dark_gray.g : white.g), point_in_rect(mouse_pos.x, mouse_pos.y, &methodDPButton) ? gray.b : (menu.method == 2 ? dark_gray.b : white.b), 255); SDL_RenderFillRect(renderer, &methodDPButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &methodDPButton); getTextDimensions(fontNormal, "Quy Hoạch Động", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Quy Hoạch Động", methodDPButton.x + (methodDPButton.w - text_render_w)/2, methodDPButton.y + (methodDPButton.h - text_render_h)/2, black, 0, false);
            renderText(renderer, fontBold, "Chọn Output:", menuStartX_Pair, menuButtonY_output - 30, black, 0, false); SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &outputScreenButton) ? gray.r : (menu.output_type == 1 ? dark_gray.r : white.r), point_in_rect(mouse_pos.x, mouse_pos.y, &outputScreenButton) ? gray.g : (menu.output_type == 1 ? dark_gray.g : white.g), point_in_rect(mouse_pos.x, mouse_pos.y, &outputScreenButton) ? gray.b : (menu.output_type == 1 ? dark_gray.b : white.b), 255); SDL_RenderFillRect(renderer, &outputScreenButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &outputScreenButton); getTextDimensions(fontNormal, "Hiện ra màn hình", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Hiện ra màn hình", outputScreenButton.x + (outputScreenButton.w - text_render_w)/2, outputScreenButton.y + (outputScreenButton.h - text_render_h)/2, black, 0, false);
            SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &outputFileButton) ? gray.r : (menu.output_type == 2 ? dark_gray.r : white.r), point_in_rect(mouse_pos.x, mouse_pos.y, &outputFileButton) ? gray.g : (menu.output_type == 2 ? dark_gray.g : white.g), point_in_rect(mouse_pos.x, mouse_pos.y, &outputFileButton) ? gray.b : (menu.output_type == 2 ? dark_gray.b : white.b), 255); SDL_RenderFillRect(renderer, &outputFileButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &outputFileButton); char outputFileTextRender[50]; if (menu.output_type == 2 && menu.output_file_choice > 0) snprintf(outputFileTextRender, sizeof(outputFileTextRender), "Output File (%d)", menu.output_file_choice); else strcpy(outputFileTextRender, "Ghi ra File"); getTextDimensions(fontNormal, outputFileTextRender, black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, outputFileTextRender, outputFileButton.x + (outputFileButton.w - text_render_w)/2, outputFileButton.y + (outputFileButton.h - text_render_h)/2, black, 0, false);
            renderText(renderer, fontBold, "Hiển thị Đồ Thị:", menuStartX_Pair, menuButtonY_graph - 30, black, 0, false); SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &showGraphButton) ? gray.r : (menu.show_graph ? dark_gray.r : white.r), point_in_rect(mouse_pos.x, mouse_pos.y, &showGraphButton) ? gray.g : (menu.show_graph ? dark_gray.g : white.g), point_in_rect(mouse_pos.x, mouse_pos.y, &showGraphButton) ? gray.b : (menu.show_graph ? dark_gray.b : white.b), 255); SDL_RenderFillRect(renderer, &showGraphButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &showGraphButton); getTextDimensions(fontNormal, "Hiển thị Đồ Thị", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Hiển thị Đồ Thị", showGraphButton.x + (showGraphButton.w - text_render_w)/2, showGraphButton.y + (showGraphButton.h - text_render_h)/2, black, 0, false);
            SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &hideGraphButton) ? gray.r : (!menu.show_graph ? dark_gray.r : white.r), point_in_rect(mouse_pos.x, mouse_pos.y, &hideGraphButton) ? gray.g : (!menu.show_graph ? dark_gray.g : white.g), point_in_rect(mouse_pos.x, mouse_pos.y, &hideGraphButton) ? gray.b : (!menu.show_graph ? dark_gray.b : white.b), 255); SDL_RenderFillRect(renderer, &hideGraphButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &hideGraphButton); getTextDimensions(fontNormal, "Không Hiển thị", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Không Hiển thị", hideGraphButton.x + (hideGraphButton.w - text_render_w)/2, hideGraphButton.y + (hideGraphButton.h - text_render_h)/2, black, 0, false);
            SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &runButton) ? gray.r : white.r, point_in_rect(mouse_pos.x, mouse_pos.y, &runButton) ? gray.g : white.g, point_in_rect(mouse_pos.x, mouse_pos.y, &runButton) ? gray.b : white.b, 255); SDL_RenderFillRect(renderer, &runButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &runButton); getTextDimensions(fontBold, "Chạy", black, &text_render_w, &text_render_h); renderText(renderer, fontBold, "Chạy", runButton.x + (runButton.w - text_render_w)/2, runButton.y + (runButton.h - text_render_h)/2, black, 0, false);
            SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &backButton) ? gray.r : white.r, point_in_rect(mouse_pos.x, mouse_pos.y, &backButton) ? gray.g : white.g, point_in_rect(mouse_pos.x, mouse_pos.y, &backButton) ? gray.b : white.b, 255); SDL_RenderFillRect(renderer, &backButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &backButton); getTextDimensions(fontNormal, "Quay lại", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Quay lại", backButton.x + (backButton.w - text_render_w)/2, backButton.y + (backButton.h - text_render_h)/2, black, 0, false);
        } else if (screen == INPUT_FILES || screen == OUTPUT_FILES) {
            const char* screenTitleTextRender = (screen == INPUT_FILES) ? "CHỌN FILE INPUT" : "CHỌN FILE OUTPUT"; renderText(renderer, fontBold, screenTitleTextRender, 0, 50, black, SCREEN_WIDTH, true); for (int i = 0; i < 6; i++) { char fileNameRender[20]; snprintf(fileNameRender, sizeof(fileNameRender), (screen == INPUT_FILES) ? "in_put%d.txt" : "out_put%d.txt", i + 1); bool isSelectedRender = (screen == INPUT_FILES && menu.input_file_choice == i + 1) || (screen == OUTPUT_FILES && menu.output_file_choice == i + 1); SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &fileButtons[i]) ? gray.r : (isSelectedRender ? dark_gray.r : white.r), point_in_rect(mouse_pos.x, mouse_pos.y, &fileButtons[i]) ? gray.g : (isSelectedRender ? dark_gray.g : white.g), point_in_rect(mouse_pos.x, mouse_pos.y, &fileButtons[i]) ? gray.b : (isSelectedRender ? dark_gray.b : white.b), 255); SDL_RenderFillRect(renderer, &fileButtons[i]); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &fileButtons[i]); getTextDimensions(fontNormal, fileNameRender, black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, fileNameRender, fileButtons[i].x + (fileButtons[i].w - text_render_w)/2, fileButtons[i].y + (fileButtons[i].h - text_render_h)/2, black, 0, false); } SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &backButtonFileSelection) ? gray.r : white.r, point_in_rect(mouse_pos.x, mouse_pos.y, &backButtonFileSelection) ? gray.g : white.g, point_in_rect(mouse_pos.x, mouse_pos.y, &backButtonFileSelection) ? gray.b : white.b, 255); SDL_RenderFillRect(renderer, &backButtonFileSelection); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &backButtonFileSelection); getTextDimensions(fontNormal, "Quay lại", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Quay lại", backButtonFileSelection.x + (backButtonFileSelection.w - text_render_w)/2, backButtonFileSelection.y + (backButtonFileSelection.h - text_render_h)/2, black, 0, false);

        } else if (screen == MANUAL_INPUT) {
            renderText(renderer, fontBold, "NHẬP THỦ CÔNG", 0, 50, black, SCREEN_WIDTH, true);
            char prompt_text_manual_orig[200]; snprintf(prompt_text_manual_orig, sizeof(prompt_text_manual_orig), "Nhập số thành phố (3-%d) và ma trận chi phí.", MAX);
            char temp_prompt_for_strtok[200]; strncpy(temp_prompt_for_strtok, prompt_text_manual_orig, sizeof(temp_prompt_for_strtok)-1); temp_prompt_for_strtok[sizeof(temp_prompt_for_strtok)-1] = '\0';
            char* line_prompt = strtok(temp_prompt_for_strtok, "\n"); int prompt_y = 90;
            while(line_prompt != NULL) { renderText(renderer, fontNormal, line_prompt, 0, prompt_y, black, SCREEN_WIDTH, true); prompt_y += TTF_FontHeight(fontNormal) - 2; line_prompt = strtok(NULL, "\n"); }
            if (strlen(manual_input.error_msg) > 0) { getTextDimensions(fontSmall, manual_input.error_msg, red_color, &text_render_w, &text_render_h); renderText(renderer, fontSmall, manual_input.error_msg, (SCREEN_WIDTH - text_render_w)/2, inputBox.y - text_render_h - 5, red_color, 0, false); }

            SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255); SDL_RenderFillRect(renderer, &inputBox);
            SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &inputBox);

            int line_h = TTF_FontHeight(fontNormal) - 2;
            int text_area_top_padding = 5; int text_area_left_padding = 5;
            int visible_text_area_height = inputBox.h - text_area_top_padding * 2;
            int visible_text_area_width = inputBox.w - text_area_left_padding * 2;

            int cursor_x_on_canvas_render, cursor_y_on_canvas_line_start_render;
            int current_total_text_height_render;
            calculate_cursor_and_total_height(fontNormal, manual_input.buffer, manual_input.buffer_len,inputBox.x + text_area_left_padding, inputBox.y + text_area_top_padding, line_h, &cursor_x_on_canvas_render, &cursor_y_on_canvas_line_start_render, &current_total_text_height_render);
            if(manual_input.buffer_len == 0) manual_input.total_text_height = line_h;
            else manual_input.total_text_height = current_total_text_height_render;

            int max_scroll_y_render = manual_input.total_text_height - visible_text_area_height;
            if (max_scroll_y_render < 0) max_scroll_y_render = 0;
            if (manual_input.scroll_offset_y > max_scroll_y_render) manual_input.scroll_offset_y = max_scroll_y_render;
            if (manual_input.scroll_offset_y < 0) manual_input.scroll_offset_y = 0;

            if (manual_input.scroll_offset_x < 0) manual_input.scroll_offset_x = 0;

            SDL_Rect text_clip_rect = { inputBox.x + text_area_left_padding, inputBox.y + text_area_top_padding,
                                        visible_text_area_width, visible_text_area_height };
            SDL_RenderSetClipRect(renderer, &text_clip_rect);

            char line_to_render_manual[sizeof(manual_input.buffer)];
            int current_char_idx_render = 0;
            int current_line_canvas_y_start = inputBox.y + text_area_top_padding; 

            while(current_char_idx_render <= manual_input.buffer_len) {
                int next_newline_idx = -1;
                for(int j = current_char_idx_render; j < manual_input.buffer_len; ++j) {
                    if (manual_input.buffer[j] == '\n') { next_newline_idx = j; break; }
                }
                int line_len = (next_newline_idx != -1) ? (next_newline_idx - current_char_idx_render) : (manual_input.buffer_len - current_char_idx_render);
                if (line_len < 0) line_len = 0;
                if (line_len >= sizeof(line_to_render_manual)) line_len = sizeof(line_to_render_manual) - 1;
                strncpy(line_to_render_manual, &manual_input.buffer[current_char_idx_render], line_len);
                line_to_render_manual[line_len] = '\0';

                int y_on_screen = current_line_canvas_y_start - manual_input.scroll_offset_y;

                int x_on_screen_for_line = text_clip_rect.x - manual_input.scroll_offset_x;

                if (y_on_screen < text_clip_rect.y + text_clip_rect.h && y_on_screen + line_h > text_clip_rect.y) {
                     renderText(renderer, fontNormal, line_to_render_manual, x_on_screen_for_line, y_on_screen, black, 0, false);
                }
                current_line_canvas_y_start += line_h;
                if (next_newline_idx != -1) current_char_idx_render = next_newline_idx + 1;
                else break;
                if (current_char_idx_render > manual_input.buffer_len && manual_input.buffer_len > 0) break;
            }

            if ((SDL_GetTicks() / 500) % 2) { 
                int cursor_visual_x_on_screen = cursor_x_on_canvas_render - manual_input.scroll_offset_x;
                int cursor_visual_y_on_screen = cursor_y_on_canvas_line_start_render - manual_input.scroll_offset_y;

                if (cursor_visual_y_on_screen >= text_clip_rect.y &&
                    cursor_visual_y_on_screen < text_clip_rect.y + text_clip_rect.h &&
                    cursor_visual_x_on_screen >= text_clip_rect.x &&
                    cursor_visual_x_on_screen < text_clip_rect.x + text_clip_rect.w ) {
                    SDL_Rect cursor_rect_manual = {cursor_visual_x_on_screen, cursor_visual_y_on_screen, 2, line_h};
                    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
                    SDL_RenderFillRect(renderer, &cursor_rect_manual);
                }
            }
            
            SDL_RenderSetClipRect(renderer, NULL); 
            SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &submitButton) ? gray.r : white.r, point_in_rect(mouse_pos.x, mouse_pos.y, &submitButton) ? gray.g : white.g, point_in_rect(mouse_pos.x, mouse_pos.y, &submitButton) ? gray.b : white.b, 255); SDL_RenderFillRect(renderer, &submitButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &submitButton); getTextDimensions(fontNormal, "Xác nhận", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Xác nhận", submitButton.x + (submitButton.w - text_render_w)/2, submitButton.y + (submitButton.h - text_render_h)/2, black, 0, false);
            SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &backButtonManual) ? gray.r : white.r, point_in_rect(mouse_pos.x, mouse_pos.y, &backButtonManual) ? gray.g : white.g, point_in_rect(mouse_pos.x, mouse_pos.y, &backButtonManual) ? gray.b : white.b, 255); SDL_RenderFillRect(renderer, &backButtonManual); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &backButtonManual); getTextDimensions(fontNormal, "Quay lại Menu", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Quay lại Menu", backButtonManual.x + (backButtonManual.w - text_render_w)/2, backButtonManual.y + (backButtonManual.h - text_render_h)/2, black, 0, false);

        } else if (screen == RESULT) {
            renderText(renderer, fontBold, "KẾT QUẢ THUẬT TOÁN", 0, 20, black, SCREEN_WIDTH, true); if (ans == INT_MAX || n == 0) { const char* noPathMsgResult = (n == 0 && !menu.manual_input_done && menu.input_file_choice == 0) ? "Chưa có dữ liệu đầu vào!" : (n==0 && (menu.manual_input_done || menu.input_file_choice > 0) && ans == INT_MAX) ? "Lỗi xử lý dữ liệu hoặc không tìm thấy đường đi!" : (n==0) ? "Lỗi đọc dữ liệu đầu vào hoặc số đỉnh không hợp lệ!" : "Không tìm được đường đi!"; getTextDimensions(fontNormal, noPathMsgResult, red_color, &text_render_w, &text_render_h); renderText(renderer, fontNormal, noPathMsgResult, (SCREEN_WIDTH - text_render_w)/2, 60, red_color, 0, false); } else { char costTextResult[100]; snprintf(costTextResult, sizeof(costTextResult), "Chi phí nhỏ nhất tìm được: %d", ans); getTextDimensions(fontNormal, costTextResult, black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, costTextResult, (SCREEN_WIDTH - text_render_w)/2, 60, black, 0, false); char pathStringResult[MAX * 6 + 50]; if (n > 0) { strcpy(pathStringResult, "Đường đi: "); int current_len_result = strlen(pathStringResult); for (int i = 0; i < n; i++) { current_len_result += snprintf(pathStringResult + current_len_result, sizeof(pathStringResult) - current_len_result, "%d -> ", path[i] + 1); } snprintf(pathStringResult + current_len_result, sizeof(pathStringResult) - current_len_result, "%d", path[0] + 1); getTextDimensions(fontNormal, pathStringResult, black, &text_render_w, &text_render_h); if (text_render_w > SCREEN_WIDTH - 40) { renderText(renderer, fontSmall, pathStringResult, 20, 95, black, 0, false); } else { renderText(renderer, fontNormal, pathStringResult, (SCREEN_WIDTH - text_render_w)/2, 95, black, 0, false); } } if (menu.show_graph) { renderGraph(renderer, fontSmall, red_color, blue_color, black, green_cost, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, 140); } } SDL_SetRenderDrawColor(renderer, point_in_rect(mouse_pos.x, mouse_pos.y, &backButton) ? gray.r : white.r, point_in_rect(mouse_pos.x, mouse_pos.y, &backButton) ? gray.g : white.g, point_in_rect(mouse_pos.x, mouse_pos.y, &backButton) ? gray.b : white.b, 255); SDL_RenderFillRect(renderer, &backButton); SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a); SDL_RenderDrawRect(renderer, &backButton); getTextDimensions(fontNormal, "Về Menu", black, &text_render_w, &text_render_h); renderText(renderer, fontNormal, "Về Menu", backButton.x + (backButton.w - text_render_w)/2, backButton.y + (backButton.h - text_render_h)/2, black, 0, false);
        }


        if (menu.run_triggered) {
            bool can_run_trigger = true; strcpy(statusMessage,"");
            if (menu.input_type == 0) { strcpy(statusMessage, "Lỗi: Vui lòng chọn phương thức Input!"); can_run_trigger = false; } else if (menu.input_type == 1 && !menu.manual_input_done) { strcpy(statusMessage, "Lỗi: Vui lòng hoàn tất nhập thủ công hoặc có lỗi!"); can_run_trigger = false; } else if (menu.input_type == 2 && menu.input_file_choice == 0) { strcpy(statusMessage, "Lỗi: Vui lòng chọn File Input!"); can_run_trigger = false; }
            if (can_run_trigger && menu.method == 0) { strcpy(statusMessage, "Lỗi: Vui lòng chọn Thuật toán!"); can_run_trigger = false; } if (can_run_trigger && menu.output_type == 0) { strcpy(statusMessage, "Lỗi: Vui lòng chọn phương thức Output!"); can_run_trigger = false; } else if (can_run_trigger && menu.output_type == 2 && menu.output_file_choice == 0) { strcpy(statusMessage, "Lỗi: Vui lòng chọn File Output!"); can_run_trigger = false; }
            if (can_run_trigger) {
                SDL_Color tempStatusColor = black; getTextDimensions(fontBold, "Đang xử lý...", tempStatusColor, &text_render_w, &text_render_h); SDL_Rect tempStatusRect = {(SCREEN_WIDTH - text_render_w)/2 - 5, 60 -2 , text_render_w + 10, text_render_h + 4}; SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a); SDL_RenderFillRect(renderer, &tempStatusRect); renderText(renderer, fontBold, "Đang xử lý...", (SCREEN_WIDTH - text_render_w)/2, 60, tempStatusColor, 0, false); SDL_RenderPresent(renderer);
                ans = INT_MAX; for(int i_path=0; i_path<MAX; ++i_path) path[i_path] = 0;
                if (menu.input_type == 1 && menu.manual_input_done) { if (n == 0 && strlen(statusMessage) == 0) { snprintf(statusMessage, sizeof(statusMessage), "Lỗi: Dữ liệu nhập thủ công không hợp lệ."); } }
                else if (menu.input_type == 2) { char inputFileNameRun[30]; snprintf(inputFileNameRun, sizeof(inputFileNameRun), "in_put%d.txt", menu.input_file_choice); input_file(inputFileNameRun, statusMessage); /* Status message is set by input_file on error */ }
                if (n > 0) { bool output_to_file_flag_run = (menu.output_type == 2); if (menu.method == 1) b_and_b(output_to_file_flag_run); else if (menu.method == 2) dynamic_program(output_to_file_flag_run); if (ans == INT_MAX && strlen(statusMessage)==0) { strcpy(statusMessage, "Không tìm được đường đi hợp lệ."); } else if (ans != INT_MAX && strlen(statusMessage)==0) { strcpy(statusMessage, "Hoàn thành!"); } if (menu.output_type == 2) { char outputFileNameRun[30]; snprintf(outputFileNameRun, sizeof(outputFileNameRun), "out_put%d.txt", menu.output_file_choice); output_file(outputFileNameRun); screen = MENU; } else { screen = RESULT; }
                } else { if (strlen(statusMessage)==0) { strcpy(statusMessage, "Lỗi: Dữ liệu đầu vào không hợp lệ hoặc không có đỉnh nào."); } screen = MENU; }
            } else { screen = MENU; }
            menu.run_triggered = false;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if (logoTexture) SDL_DestroyTexture(logoTexture);
    TTF_CloseFont(fontTitle); TTF_CloseFont(fontBold); TTF_CloseFont(fontNormal); TTF_CloseFont(fontSmall);
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
    TTF_Quit(); IMG_Quit(); SDL_Quit();
    return 0;
}

void SDL_FakeMouseButtonEvent(SDL_Rect* button, Uint8 button_type) { 
    SDL_Event event_fake; SDL_zero(event_fake); event_fake.type = SDL_MOUSEBUTTONDOWN; event_fake.button.button = button_type; event_fake.button.x = button->x + button->w / 2; event_fake.button.y = button->y + button->h / 2; event_fake.button.timestamp = SDL_GetTicks(); SDL_Window* current_window = NULL; if(SDL_GetMouseFocus() != NULL) { current_window = SDL_GetMouseFocus(); } else if (SDL_GL_GetCurrentWindow() != NULL) { current_window = SDL_GL_GetCurrentWindow(); } if (current_window) { event_fake.button.windowID = SDL_GetWindowID(current_window); } SDL_PushEvent(&event_fake);
}