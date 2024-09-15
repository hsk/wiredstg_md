// App.h : アプリケーション
// 状態
#define APP_STATE_NULL                  0
#define APP_STATE_TITLE_INITIALIZE      1
#define APP_STATE_TITLE_UPDATE          2
#define APP_STATE_GAME_INITIALIZE       3
#define APP_STATE_GAME_UPDATE           4
// 外部関数宣言
void AppInitialize(void);
void AppUpdate(void);
// 外部変数宣言
extern char appState;// 状態
