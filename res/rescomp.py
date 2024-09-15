import copy,os,sys,json
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"
import pygame

def load_image(filename):
    return pygame.image.load(filename)

class Pal:
    def to_md_color(self,col):
        return (((col.b+8)//17)<<8)+(((col.g+8)//17)<<4)+((col.r+8)//17)
    def print_colors(self,img):
        colors = img.get_palette()
        for i in range(0,64):
            if i & 15 == 0: print("    ",end="")
            print(f"0x{self.to_md_color(colors[i]):04x}",end=", " if (i & 15) != 15 else ",\n")
    def print_pal(self,name,img):
        print(f"extern u16 const {name}[64];")
        print(f"#ifdef IMPL_DATA")
        print(f"u16 const {name}[64] = {{")
        self.print_colors(img)
        print("};")
        print(f"#endif")
# 画像を読み込みタイルの出力のみ行うもの
class TileSet:
    # タイル単体の出力
    def print_tile(self,img,pos):
        for y in range(0,8):
            print("    0x",end="")
            for x in range(0,8):
                print(f"{(img.get_at_mapped((x+pos[0],y+pos[1]))&0xf):x}",end="")
            print(",")
    # 画像のタイル出力
    def print_tiles(self,name,img):
        w,h = img.get_size()
        size = (w//8)*(h//8)*8
        print(f"extern u32 const {name}[{size}];")
        print(f"#ifdef IMPL_DATA")
        print(f"u32 const {name}[{size}] = {{")
        i = 0
        for y in range(0,h,8):
            for x in range(0,w,8):
                print(f"    // {i}: {x//8,y//8}")
                self.print_tile(img,(x,y))
                i += 1
        print("};")
        print(f"#endif")
class Image:
    def get_ptile(self,img,pos):
        pal = 0
        tile = []
        for y in range(0,8):
            xs = ""
            for x in range(0,8):
                p = img.get_at_mapped((pos[0]+x,pos[1]+y))
                xs+=f"{(p&0xf):x}"
                pal |= p>>4
            tile.append(xs)
        return pal,tile
    def check_tile(self,tile):
        t = f"{tile}"
        if t in self.tileIndexs: return self.tileIndexs[t]
        return None
    def flip(self,tile,xflip,yflip):
        if yflip: tile = list(reversed(tile))
        if xflip:
            t2=[]
            for t in tile:
                t2.append(t[::-1])
            return t2
        return tile
    def print_tile(self,i,tile):
        print(f"    // {i:d} 0x{i:x}")
        for y in tile: print(f"    0x{y},")
    def print_tiles(self,name,img):
        tiles = []
        self.tileIndexs = {}
        map = []
        w,h = img.get_size()
        size = (w//8)*(h//8)*4*8
        for y in range(0,h,8):
            for x in range(0,w,8):
                ptile=self.get_ptile(img,(x,y))
                ret=self.check_tile(ptile[1])
                if ret == None:
                    for xf in range(2):
                        for yf in range(2):
                            k = f"{self.flip(ptile[1],xf,yf)}"
                            if not k in self.tileIndexs:
                                self.tileIndexs[k]=(xf+yf*2)*2048+len(tiles)
                    ret = len(tiles)
                    tiles.append(ptile[1])
                map.append(ptile[0]*4*2048|ret)
        print(f"extern u32 const {name}_tiles[{len(tiles)}*8];")
        print("#ifdef IMPL_DATA")
        print(f"u32 const {name}_tiles[{len(tiles)}*8] = {{")
        for i,tile in enumerate(tiles):
            self.print_tile(i,tile)
        print(f"}};")
        print("#endif")
        return (w//8,h//8,map,len(tiles))
    def print_map(self,name,img):
        w,h,map,tile_num = self.print_tiles(name,img)
        print(f"extern u16 const {name}_map[{w}*{h}];")
        print("#ifdef IMPL_DATA")
        print(f"u16 const {name}_map[{w}*{h}] = {{")
        for i,m in enumerate(map):
            if i % w == 0: print("    ",end="")
            print(f"0x{m:04x},",end="" if i % w != w-1 else "\n")
        print("};")
        print("#endif")
    def print_image(self,name,img):
        Pal().print_pal(name+"_pal",img)
        self.print_map(name,img)

# tmjを読み込んで、classに入れる
# posとcolは自動生成する
# class からアセンブラを出力する
# class からtmjを生成する
# スプライトは複数のスプライトに分割して登録できる。
# スプライトは配列にまとめて１つの名前で管理できる。

# スプライトクラスは位置と範囲とrectを持つ
class Sprite:
    def __init__(self,objs=[],cols=None,pos=None):
        self.objs = objs
        self.pos = pos
        self.cols = cols
        # posがNoneならobjsの中心にする
        if self.pos == None:
            x,y,w,h = 32000,32000,-65000,-65000
            for (px,py,pw,ph) in self.objs:
                x = min(x,px)
                y = min(y,py)
                w = max(w,px+pw)
                h = max(h,py+ph)
            self.pos = ((x+w)//2,(y+h)//2)
        # colsがNoneならobjsと同じにするか、サイズ80%にするなどする。
        if self.cols == None: self.cols = copy.deepcopy(self.objs)

# スプライトの集合である。self.spritesは名前からspriteのリストのdictだ。
class Sprites:
    def __init__(self,sprites={}):
        self.objs = {}
        self.poss = {}
        self.cols = {}
        self.sprites=sprites
        self.img = None
        self.img_name = None
    # add_spriteで集合にspriteを追加できる。
    def add_obj(self,name,n,obj):
        if not name in self.objs:
            self.objs[name]=[]
        while len(self.objs[name])<=n:
            self.objs[name].append([])
        self.objs[name][n].append(obj)
    def commit_obj(self):
        self.sprites = {}
        for name,arrs in self.objs.items():
            self.sprites[name]=[]
            for arr in arrs:
                self.sprites[name].append(Sprite(arr))
        self.objs = {}
    def add_pos(self,name,n,pos):
        #print(f"add pos {name} {n} {pos}")
        if not name in self.poss:
            self.poss[name]=[]
        while len(self.poss[name])<=n:
            self.poss[name].append((0,0))
        self.poss[name][n]=pos
    def commit_pos(self):
        #print(f"commit pos {self.poss}")
        for name,poss in self.poss.items():
            for n,pos in enumerate(poss):
                self.sprites[name][n].pos = pos
        self.poss = {}
    def add_col(self,name,n,obj):
        if not name in self.cols:
            self.cols[name]=[]
        while len(self.cols[name])<=n:
            self.cols[name].append([])
        self.cols[name][n].append(obj)
    def commit_col(self):
        for name,colss in self.cols.items():
            for n,cols in enumerate(colss):
                self.sprites[name][n].cols = cols
        self.cols = {}

    def load_json(self, file):
        with open(file) as f:
            return json.load(f)

    def save_json(self, file,dict,format=False):
        with open(file, "w") as fp:
            if format: fp.write(self.dump_json(dict))
            else: json.dump(dict,fp)
    def dump_json(self, dict):
        class MyJSONEncoder(json.JSONEncoder):
            def iterencode(self, o, _one_shot=False):
                list_lvl = 0
                for s in super(MyJSONEncoder, self).iterencode(o, _one_shot=_one_shot):
                    if s.startswith('['):
                        list_lvl += 1
                        s = s.replace('\n', '').rstrip()
                    elif 0 < list_lvl:
                        s = s.replace('\n', '').rstrip()
                        if s and s[-1] == ',':
                            s = s[:-1] + self.item_separator
                        elif s and s[-1] == ':':
                            s = s[:-1] + self.key_separator
                    if s.endswith(']'):
                        list_lvl -= 1
                    yield s
        return json.dumps(dict, indent=4,cls=MyJSONEncoder)

    def find_name(self,array,name,tag="name"):
        for a in array:
            if a[tag]==name: return a
    def find_all(self,array,name,tag="name"):
        rc = []
        for a in array:
            if a[tag]==name: rc.append(a)
        return rc

    def load_tmj(self,filename):
        # jsonを読み込み
        #jsonread
        json = self.load_json(filename)
        # 画像名取得
        self.img_name=self.find_name(json["layers"],"img")["image"]
        self.img = load_image(self.img_name)
        # bgレイヤー読み込み
        bg = self.find_name(json["layers"],"bg")
        if bg != None:
            for p in bg["objects"]:
                name = p["name"]# 名前を取得
                sp=(p["x"],p["y"],p["width"],p["height"])# 範囲を取得
                Image().print_image(name,self.img.subsurface(sp))
        # objのレイヤーを読み込む
        obj = self.find_name(json["layers"],"obj")
        # objのレイヤーをループして
        if obj == None: return False
        for p in obj["objects"]:
            # 名前を取得
            name = p["name"].split("_")[0]
            # 番号を取得
            n = int(p["name"].split("_")[1])
            # 範囲を取得
            sp=(p["x"],p["y"],p["width"],p["height"])
            self.add_obj(name,n,sp)
        self.commit_obj()
        # posのレイヤーを読み込む
        poss = self.find_name(json["layers"],"pos")
        # posのレイヤーがあればループ
        if poss != None:
            for p in poss["objects"]:
                # 名前を取得
                name = p["name"].split("_")[0]
                # 番号を取得
                n = int(p["name"].split("_")[1])
                self.add_pos(name,n,(p["x"],p["y"]))
            self.commit_pos()
        # colのレイヤーを読み込む
        cols = self.find_name(json["layers"],"col")
        # posのレイヤーがあればループ
        if cols != None:
            for p in cols["objects"]:
                # 名前を取得
                name = p["name"].split("_")[0]
                # 番号を取得
                n = int(p["name"].split("_")[1])
                self.add_col(name,n,p)
            self.commit_col()
        return True
    """
    def save_tmj(self,filename):
        # テンプレートを読み込み
        json = {}
        json["image"]=self.img_name
        obj,col,pos = [],[],[]
        # オブジェクトレイヤーに追加
        for name,arrs in self.sprites:
            for n,arr in arrs:
                obj.append({"name":f"{name}_{n}","x":x,"y":y,"width":w,"height":h})
        # 当たり判定レイヤーに追加
        for name,arrs in self.sprites:
            for n,arr in arrs:
                col.append({"name":f"{name}_{n}","x":x,"y":y,"width":w,"height":h})
        # 位置レイヤーに追加
        for name,arrs in self.sprites:
            for n,arr in arrs:
                pos.append({"name":f"{name}_{n}","x":x,"y":y,"width":0,"height":0})
        self.save_json(filename,json)
    """
    def print_tile(self,img, pos):
        for y in range(0,8):
            print("    0x",end="")
            for x in range(0,8):
                print(f"{(img.get_at_mapped((x+pos[0],y+pos[1]))&0xf):x}",end="")
            print(",")
    def print_tiles(self,index,img, obj):
        i = 0
        for x in range(obj[0],obj[0]+obj[2],8):
            for y in range(obj[1],obj[1]+obj[3],8):
                print(f"    // {index+i} {i}")
                self.print_tile(img,(x,y))
                i+=1
        return (obj[2]>>3)*(obj[3]>>3)
    def print_sprite(self,rname,pos,obj,index):
        # サイズを出力
        w = (obj[2]>>3)-1
        h = (obj[3]>>3)-1
        size = (w<<2) | (h<<0)
        # インデックスを出力
        # 中央位置を出力
        x = obj[0]-pos[0]
        y = obj[1]-pos[1]
        print(f"    {{{y:4d}, 0, 0x{size:02x}, {x:4d}, 0, 0x{index:04x}}},")

        """typedef struct
        {
            s8 offsetY;          // respect VDP sprite field order, may help
            u8 offsetYFlip;
            u8 size;
            s8 offsetX;
            u8 offsetXFlip;
            u16 attribut;
        }  FrameSPR;
        """
    # c言語データを作る
    def print_c(self,rname):
        img = self.img # 画像データ読み込み
        # インクルードなどを出力
        # タイルのヘッダを出力
        size = 0
        for name,sps in self.sprites.items(): # スプライトをループ
            for n,sp in enumerate(sps): # スプライトの配列をループ
                for obj in sp.objs: #仮想スプライト内 オブジェを取得
                    size+=(obj[2]>>3)*(obj[3]>>3)
        print(f"extern u32 const {rname}_tiles[{size}*8];")
        print("#ifdef IMPL_DATA")
        print(f"u32 const {rname}_tiles[{size}*8] = {{")
        index = 0        
        indexs = []
        # タイルを出力
        for name,sps in self.sprites.items(): # スプライトをループ
            for n,sp in enumerate(sps): # スプライトの配列をループ
                for obj in sp.objs: #仮想スプライト内 オブジェを取得
                    indexs.append(index)
                    index += self.print_tiles(index,img, obj)
        # タイルのフッタを出力
        print("};")
        print("#endif")
        #print(f"index {indexs}")
        # 仮想スプライト配列の出力
        print(f"extern FrameSPR const {rname}_framesprs[{len(indexs)}];")
        print("#ifdef IMPL_DATA")
        print(f"FrameSPR const {rname}_framesprs[{len(indexs)}] = {{")
        spindexs={}
        spindex = 0
        for name,sps in self.sprites.items(): # 仮想スプライトリストをループ
            spindexs[name]=[]
            for n,sp in enumerate(sps): # 仮想アニメスプライトの配列をループ
                # 仮想スプライトのヘッダを出力
                spindexs[name].append((spindex,len(sp.objs)))
                for j,obj in enumerate(sp.objs): #仮想スプライト内 オブジェを取得
                    self.print_sprite(rname,sp.pos,obj,indexs[spindex])
                    spindex+=1
                # 仮想スプライトのフッタを出力
        print("};")
        print("#endif")
        for name,idxs in spindexs.items(): # 仮想スプライトリストをループ
            print(f"extern FSPR const {name}_sprite[{len(idxs)}];")
            print("#ifdef IMPL_DATA")
            print(f"FSPR const {name}_sprite[{len(idxs)}] = {{")
            for i,idx in enumerate(idxs):
                print(f"    {{{idx[1]:2d}, {idx[0]:2d},{idx[0]:2d}, (FrameSPR*)&{rname}_framesprs[{idx[0]:2d}]}},")
            print("};")
            print("#endif")
    def print_sprites(self,name,filename):
        if self.load_tmj(filename): self.print_c(name)

match sys.argv[1]:
    case "c":
        print('#include "bios.h"')
        print('#define IMPL_DATA')
        print('#include "gfx.h"')
    case "pal":
        Pal().print_pal(sys.argv[2],load_image(sys.argv[3]))
    case "tileset":
        TileSet().print_tiles(sys.argv[2],load_image(sys.argv[3]))
    case "image":
        Image().print_image(sys.argv[2],load_image(sys.argv[3]))
    case "sprite":
        Sprites().print_sprites(sys.argv[2],sys.argv[3])
