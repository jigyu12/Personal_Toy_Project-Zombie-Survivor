#pragma once

#include <random>
#include <list>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <limits>
#include <fstream>
#include <locale>
#include <codecvt>
#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "rapidcsv.h"
#include "json.hpp"
#include "Defines.h"
#include "Variables.h"
#include "DebugBox.h"
#include "HitBox.h"
#include "Utils.h"
#include "Singleton.h"
#include "DataTable.h"
#include "StringTable.h"
#include "DataTableMgr.h"
#include "Framework.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "AnimationClip.h"
#include "Animator.h"
#include "GameObject.h"
#include "TextGo.h"
#include "SpriteGo.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "ObjectPool.h"
#include "SoundMgr.h"

using json = nlohmann::json;