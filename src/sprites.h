/*****************************************************************************
 * Copyright (c) 2014 Ted John
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * This file is part of OpenRCT2.
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef _SPRITES_H_
#define _SPRITES_H_

enum {
	SPR_NONE = -1,

	SPR_PALETTE_1_START = 3100,
	SPR_PALETTE_1_END = 3110,

	SPR_COOL_AND_WET = 3290,
	SPR_WARM = 3291,
	SPR_HOT_AND_DRY = 3292,
	SPR_COLD = 3293,

	// This is the start of every character there are 
	// 224 characters per font (first 32 are control codes hence why it doesn't go to 255)
	// 4 fonts
	// = 896 sprites
	SPR_CHAR_START = 3861,
	SPR_CHAR_END = 4757,

	SPR_PALETTE_2_START = 4915,
	SPR_PALETTE_2_END = 5047,

	SPR_RESIZE = 5058,

	SPR_PREVIOUS = 5160,
	SPR_NEXT = 5161,

	SPR_CONSTRUCTION = 5164,
	SPR_DEMOLISH = 5165,
	SPR_HEARING_VIEWPORT = 5166,
	SPR_LOCATE = 5167,
	SPR_RENAME = 5168,
	SPR_ROTATE_ARROW = 5169,

	SPR_PICKUP_BTN = 5174,
	SPR_PATROL_BTN = 5175,
	SPR_BUY_LAND_RIGHTS = 5176,
	SPR_BUY_CONSTRUCTION_RIGHTS = 5177,
	SPR_NO_ENTRY = 5178,
	SPR_CLOSED = 5179,
	SPR_OPEN = 5180,
	SPR_TESTING = 5181,
	SPR_TOGGLE_OPEN_CLOSE = 5182,

	SPR_RIDE = 5187,
	SPR_TRACK_PEEP = 5188,
	SPR_SCENERY = 5189,
	SPR_FINANCE = 5190,
	SPR_NEW_RIDE = 5191,
	SPR_MAP = 5192,
	SPR_GUESTS = 5193,
	SPR_AWARD = 5194,
	SPR_GRAPH = 5195,

	SPR_PARK_ENTRANCE = 5197,

	SPR_TAB_GRAPH_0 = 5245,
	SPR_TAB_GRAPH_1 = SPR_TAB_GRAPH_0 + 1,
	SPR_TAB_GRAPH_2 = SPR_TAB_GRAPH_0 + 2,
	SPR_TAB_GRAPH_3 = SPR_TAB_GRAPH_0 + 3,
	SPR_TAB_GRAPH_4 = SPR_TAB_GRAPH_0 + 4,
	SPR_TAB_GRAPH_5 = SPR_TAB_GRAPH_0 + 5,
	SPR_TAB_GRAPH_6 = SPR_TAB_GRAPH_0 + 6,
	SPR_TAB_GRAPH_7 = SPR_TAB_GRAPH_0 + 7,

	SPR_TAB_ADMISSION_0 = 5253,
	SPR_TAB_ADMISSION_1 = SPR_TAB_ADMISSION_0 + 1,
	SPR_TAB_ADMISSION_2 = SPR_TAB_ADMISSION_0 + 2,
	SPR_TAB_ADMISSION_3 = SPR_TAB_ADMISSION_0 + 3,
	SPR_TAB_ADMISSION_4 = SPR_TAB_ADMISSION_0 + 4,
	SPR_TAB_ADMISSION_5 = SPR_TAB_ADMISSION_0 + 5,
	SPR_TAB_ADMISSION_6 = SPR_TAB_ADMISSION_0 + 6,
	SPR_TAB_ADMISSION_7 = SPR_TAB_ADMISSION_0 + 7,

	SPR_TAB_FINANCES_SUMMARY_0 = 5261,
	SPR_TAB_FINANCES_SUMMARY_1 = SPR_TAB_FINANCES_SUMMARY_0 + 1,
	SPR_TAB_FINANCES_SUMMARY_2 = SPR_TAB_FINANCES_SUMMARY_0 + 2,
	SPR_TAB_FINANCES_SUMMARY_3 = SPR_TAB_FINANCES_SUMMARY_0 + 3,
	SPR_TAB_FINANCES_SUMMARY_4 = SPR_TAB_FINANCES_SUMMARY_0 + 4,
	SPR_TAB_FINANCES_SUMMARY_5 = SPR_TAB_FINANCES_SUMMARY_0 + 5,
	SPR_TAB_FINANCES_SUMMARY_6 = SPR_TAB_FINANCES_SUMMARY_0 + 6,
	SPR_TAB_FINANCES_SUMMARY_7 = SPR_TAB_FINANCES_SUMMARY_0 + 7,

	SPR_TAB_STATS_0 = 5277,
	SPR_TAB_STATS_1 = SPR_TAB_STATS_0 + 1,
	SPR_TAB_STATS_2 = SPR_TAB_STATS_0 + 2,
	SPR_TAB_STATS_3 = SPR_TAB_STATS_0 + 3,
	SPR_TAB_STATS_4 = SPR_TAB_STATS_0 + 4,
	SPR_TAB_STATS_5 = SPR_TAB_STATS_0 + 5,
	SPR_TAB_STATS_6 = SPR_TAB_STATS_0 + 6,

	SPR_PEEP_LARGE_FACE_VERY_VERY_UNHAPPY = 5284,
	SPR_PEEP_LARGE_FACE_VERY_UNHAPPY = 5285,
	SPR_PEEP_LARGE_FACE_UNHAPPY = 5286,
	SPR_PEEP_LARGE_FACE_NORMAL = 5287,
	SPR_PEEP_LARGE_FACE_HAPPY = 5288,
	SPR_PEEP_LARGE_FACE_VERY_HAPPY = 5289,
	SPR_PEEP_LARGE_FACE_VERY_VERY_HAPPY = 5290,
	SPR_PEEP_LARGE_FACE_TIRED = 5291,
	SPR_PEEP_LARGE_FACE_VERY_TIRED = 5292,
	SPR_PEEP_LARGE_FACE_SICK = 5293,
	SPR_PEEP_LARGE_FACE_VERY_SICK = 5294,

	SPR_PEEP_LARGE_FACE_VERY_VERY_SICK = 5298,

	SPR_PEEP_LARGE_FACE_ANGRY = 5314,

	SPR_TAB_STAFF_OPTIONS_0 = 5319,
	SPR_TAB_STAFF_OPTIONS_1 = SPR_TAB_STAFF_OPTIONS_0 + 1,
	SPR_TAB_STAFF_OPTIONS_2 = SPR_TAB_STAFF_OPTIONS_0 + 2,
	SPR_TAB_STAFF_OPTIONS_3 = SPR_TAB_STAFF_OPTIONS_0 + 3,
	SPR_TAB_STAFF_OPTIONS_4 = SPR_TAB_STAFF_OPTIONS_0 + 4,
	SPR_TAB_STAFF_OPTIONS_5 = SPR_TAB_STAFF_OPTIONS_0 + 5,
	SPR_TAB_STAFF_OPTIONS_6 = SPR_TAB_STAFF_OPTIONS_0 + 6,

	SPR_TAB_FINANCES_RESEARCH_0 = 5327,

	SPR_TAB_SHOPS_AND_STALLS_0 = 5351,
	SPR_TAB_SHOPS_AND_STALLS_1 = SPR_TAB_SHOPS_AND_STALLS_0 + 1,
	SPR_TAB_SHOPS_AND_STALLS_2 = SPR_TAB_SHOPS_AND_STALLS_0 + 2,
	SPR_TAB_SHOPS_AND_STALLS_3 = SPR_TAB_SHOPS_AND_STALLS_0 + 3,
	SPR_TAB_SHOPS_AND_STALLS_4 = SPR_TAB_SHOPS_AND_STALLS_0 + 4,
	SPR_TAB_SHOPS_AND_STALLS_5 = SPR_TAB_SHOPS_AND_STALLS_0 + 5,
	SPR_TAB_SHOPS_AND_STALLS_6 = SPR_TAB_SHOPS_AND_STALLS_0 + 6,
	SPR_TAB_SHOPS_AND_STALLS_7 = SPR_TAB_SHOPS_AND_STALLS_0 + 7,
	SPR_TAB_SHOPS_AND_STALLS_8 = SPR_TAB_SHOPS_AND_STALLS_0 + 8,
	SPR_TAB_SHOPS_AND_STALLS_9 = SPR_TAB_SHOPS_AND_STALLS_0 + 9,
	SPR_TAB_SHOPS_AND_STALLS_10 = SPR_TAB_SHOPS_AND_STALLS_0 + 10,
	SPR_TAB_SHOPS_AND_STALLS_11 = SPR_TAB_SHOPS_AND_STALLS_0 + 11,
	SPR_TAB_SHOPS_AND_STALLS_12 = SPR_TAB_SHOPS_AND_STALLS_0 + 12,
	SPR_TAB_SHOPS_AND_STALLS_13 = SPR_TAB_SHOPS_AND_STALLS_0 + 13,
	SPR_TAB_SHOPS_AND_STALLS_14 = SPR_TAB_SHOPS_AND_STALLS_0 + 14,
	SPR_TAB_SHOPS_AND_STALLS_15 = SPR_TAB_SHOPS_AND_STALLS_0 + 15,

	SPR_TAB_KIOSKS_AND_FACILITIES_0 = 5367,
	SPR_TAB_KIOSKS_AND_FACILITIES_1 = SPR_TAB_KIOSKS_AND_FACILITIES_0 + 1,
	SPR_TAB_KIOSKS_AND_FACILITIES_2 = SPR_TAB_KIOSKS_AND_FACILITIES_0 + 2,
	SPR_TAB_KIOSKS_AND_FACILITIES_3 = SPR_TAB_KIOSKS_AND_FACILITIES_0 + 3,
	SPR_TAB_KIOSKS_AND_FACILITIES_4 = SPR_TAB_KIOSKS_AND_FACILITIES_0 + 4,
	SPR_TAB_KIOSKS_AND_FACILITIES_5 = SPR_TAB_KIOSKS_AND_FACILITIES_0 + 5,
	SPR_TAB_KIOSKS_AND_FACILITIES_6 = SPR_TAB_KIOSKS_AND_FACILITIES_0 + 6,
	SPR_TAB_KIOSKS_AND_FACILITIES_7 = SPR_TAB_KIOSKS_AND_FACILITIES_0 + 7,

	SPR_TAB_FINANCES_FINANCIAL_GRAPH_0 = 5375,

	SPR_TAB_FINANCES_PROFIT_GRAPH_0 = 5391,

	SPR_TAB_FINANCES_VALUE_GRAPH_0 = 5407,

	SPR_TAB_FINANCES_MARKETING_0 = 5423,

	SPR_TAB_RIDE_0 = 5442,
	SPR_TAB_RIDE_1 = SPR_TAB_RIDE_0 + 1,
	SPR_TAB_RIDE_2 = SPR_TAB_RIDE_0 + 2,
	SPR_TAB_RIDE_3 = SPR_TAB_RIDE_0 + 3,
	SPR_TAB_RIDE_4 = SPR_TAB_RIDE_0 + 4,
	SPR_TAB_RIDE_5 = SPR_TAB_RIDE_0 + 5,
	SPR_TAB_RIDE_6 = SPR_TAB_RIDE_0 + 6,
	SPR_TAB_RIDE_7 = SPR_TAB_RIDE_0 + 7,
	SPR_TAB_RIDE_8 = SPR_TAB_RIDE_0 + 8,
	SPR_TAB_RIDE_9 = SPR_TAB_RIDE_0 + 9,
	SPR_TAB_RIDE_10 = SPR_TAB_RIDE_0 + 10,
	SPR_TAB_RIDE_11 = SPR_TAB_RIDE_0 + 11,
	SPR_TAB_RIDE_12 = SPR_TAB_RIDE_0 + 12,
	SPR_TAB_RIDE_13 = SPR_TAB_RIDE_0 + 13,
	SPR_TAB_RIDE_14 = SPR_TAB_RIDE_0 + 14,
	SPR_TAB_RIDE_15 = SPR_TAB_RIDE_0 + 15,

	STR_TAB_PARK = 5466,

	SPR_AWARD_MOST_UNTIDY = 5469,
	SPR_MOST_TIDY = SPR_AWARD_MOST_UNTIDY + 1,
	SPR_BEST_ROLLERCOASTERS = SPR_AWARD_MOST_UNTIDY + 2,
	SPR_BEST_VALUE = SPR_AWARD_MOST_UNTIDY + 3,
	SPR_MOST_BEAUTIFUL = SPR_AWARD_MOST_UNTIDY + 4,
	SPR_WORST_VALUE = SPR_AWARD_MOST_UNTIDY + 5,
	SPR_SAFEST = SPR_AWARD_MOST_UNTIDY + 6,
	SPR_BEST_STAFF = SPR_AWARD_MOST_UNTIDY + 7,
	SPR_BEST_FOOD = SPR_AWARD_MOST_UNTIDY + 8,
	SPR_WORST_FOOD = SPR_AWARD_MOST_UNTIDY + 9,
	SPR_BEST_RESTROOMS = SPR_AWARD_MOST_UNTIDY + 10,
	SPR_MOST_DISAPPOINTING = SPR_AWARD_MOST_UNTIDY + 11,
	SPR_BEST_WATER_RIDES = SPR_AWARD_MOST_UNTIDY + 12,
	SPR_BEST_CUSTOM_DESIGNED_RIDES = SPR_AWARD_MOST_UNTIDY + 13,
	SPR_MOST_DAZZLING_RIDE_COLOURS = SPR_AWARD_MOST_UNTIDY + 14,
	SPR_MOST_CONFUSING_LAYOUT = SPR_AWARD_MOST_UNTIDY + 15,
	SPR_BEST_GENTLE_RIDES = SPR_AWARD_MOST_UNTIDY + 16,

	SPR_PEEP_SMALL_FACE_VERY_VERY_UNHAPPY = 5486,
	SPR_PEEP_SMALL_FACE_VERY_UNHAPPY = 5487,
	SPR_PEEP_SMALL_FACE_UNHAPPY = 5488,
	SPR_PEEP_SMALL_FACE_NORMAL = 5489,
	SPR_PEEP_SMALL_FACE_HAPPY = 5490,
	SPR_PEEP_SMALL_FACE_VERY_HAPPY = 5491,
	SPR_PEEP_SMALL_FACE_VERY_VERY_HAPPY = 5492,
	SPR_PEEP_SMALL_FACE_TIRED = 5493,
	SPR_PEEP_SMALL_FACE_VERY_TIRED = 5494,
	SPR_PEEP_SMALL_FACE_SICK = 5495,
	SPR_PEEP_SMALL_FACE_VERY_SICK = 5496,
	SPR_PEEP_SMALL_FACE_VERY_VERY_SICK = 5497,
	SPR_PEEP_SMALL_FACE_ANGRY = 5498,

	SPR_LAND_TOOL_DECREASE = 5499,

	SPR_LAND_TOOL_INCREASE = 5501,

	SPR_LAND_TOOL_SIZE_0 = 5503,
	SPR_LAND_TOOL_SIZE_1 = SPR_LAND_TOOL_SIZE_0 + 1,
	SPR_LAND_TOOL_SIZE_2 = SPR_LAND_TOOL_SIZE_0 + 2,
	SPR_LAND_TOOL_SIZE_3 = SPR_LAND_TOOL_SIZE_0 + 3,
	SPR_LAND_TOOL_SIZE_4 = SPR_LAND_TOOL_SIZE_0 + 4,
	SPR_LAND_TOOL_SIZE_5 = SPR_LAND_TOOL_SIZE_0 + 5,
	SPR_LAND_TOOL_SIZE_6 = SPR_LAND_TOOL_SIZE_0 + 6,
	SPR_LAND_TOOL_SIZE_7 = SPR_LAND_TOOL_SIZE_0 + 7,

	SPR_TAB_OBJECTIVE_0 = 5511,
	SPR_TAB_OBJECTIVE_1 = SPR_TAB_OBJECTIVE_0 + 1,
	SPR_TAB_OBJECTIVE_2 = SPR_TAB_OBJECTIVE_0 + 2,
	SPR_TAB_OBJECTIVE_3 = SPR_TAB_OBJECTIVE_0 + 3,
	SPR_TAB_OBJECTIVE_4 = SPR_TAB_OBJECTIVE_0 + 4,
	SPR_TAB_OBJECTIVE_5 = SPR_TAB_OBJECTIVE_0 + 5,
	SPR_TAB_OBJECTIVE_6 = SPR_TAB_OBJECTIVE_0 + 6,
	SPR_TAB_OBJECTIVE_7 = SPR_TAB_OBJECTIVE_0 + 7,
	SPR_TAB_OBJECTIVE_8 = SPR_TAB_OBJECTIVE_0 + 8,
	SPR_TAB_OBJECTIVE_9 = SPR_TAB_OBJECTIVE_0 + 9,
	SPR_TAB_OBJECTIVE_10 = SPR_TAB_OBJECTIVE_0 + 10,
	SPR_TAB_OBJECTIVE_11 = SPR_TAB_OBJECTIVE_0 + 11,
	SPR_TAB_OBJECTIVE_12 = SPR_TAB_OBJECTIVE_0 + 12,
	SPR_TAB_OBJECTIVE_13 = SPR_TAB_OBJECTIVE_0 + 13,
	SPR_TAB_OBJECTIVE_14 = SPR_TAB_OBJECTIVE_0 + 14,
	SPR_TAB_OBJECTIVE_15 = SPR_TAB_OBJECTIVE_0 + 15,

	SPR_TAB_AWARDS = 5527,
	SPR_TAB_QUESTION = 5528,

	SPR_TAB_GUESTS_0 = 5568,
	SPR_TAB_GUESTS_1 = SPR_TAB_GUESTS_0 + 1,
	SPR_TAB_GUESTS_2 = SPR_TAB_GUESTS_0 + 2,
	SPR_TAB_GUESTS_3 = SPR_TAB_GUESTS_0 + 3,
	SPR_TAB_GUESTS_4 = SPR_TAB_GUESTS_0 + 4,
	SPR_TAB_GUESTS_5 = SPR_TAB_GUESTS_0 + 5,
	SPR_TAB_GUESTS_6 = SPR_TAB_GUESTS_0 + 6,
	SPR_TAB_GUESTS_7 = SPR_TAB_GUESTS_0 + 7,
	SPR_TAB_GUESTS_8 = SPR_TAB_GUESTS_0 + 8,
	SPR_TAB_GUESTS_9 = SPR_TAB_GUESTS_0 + 9,
	SPR_TAB_GUESTS_10 = SPR_TAB_GUESTS_0 + 10,
	SPR_TAB_GUESTS_11 = SPR_TAB_GUESTS_0 + 11,
	SPR_TAB_GUESTS_12 = SPR_TAB_GUESTS_0 + 12,
	SPR_TAB_GUESTS_13 = SPR_TAB_GUESTS_0 + 13,
	SPR_TAB_GUESTS_14 = SPR_TAB_GUESTS_0 + 14,
	SPR_TAB_GUESTS_15 = SPR_TAB_GUESTS_0 + 15,

	SPR_FLOOR_TEXTURE_GRASS = 5579,
	SPR_FLOOR_TEXTURE_SAND = SPR_FLOOR_TEXTURE_GRASS + 1,
	SPR_FLOOR_TEXTURE_DIRT = SPR_FLOOR_TEXTURE_GRASS + 2,
	SPR_FLOOR_TEXTURE_ROCK = SPR_FLOOR_TEXTURE_GRASS + 3,
	SPR_FLOOR_TEXTURE_MARTIAN = SPR_FLOOR_TEXTURE_GRASS + 4,
	SPR_FLOOR_TEXTURE_CHECKERBOARD = SPR_FLOOR_TEXTURE_GRASS + 5,
	SPR_FLOOR_TEXTURE_GRASS_CLUMPS = SPR_FLOOR_TEXTURE_GRASS + 6,
	SPR_FLOOR_TEXTURE_ICE = SPR_FLOOR_TEXTURE_GRASS + 7,
	SPR_FLOOR_TEXTURE_GRID_RED = SPR_FLOOR_TEXTURE_GRASS + 8,
	SPR_FLOOR_TEXTURE_GRID_YELLOW = SPR_FLOOR_TEXTURE_GRASS + 9,
	SPR_FLOOR_TEXTURE_GRID_BLUE = SPR_FLOOR_TEXTURE_GRASS + 10,
	SPR_FLOOR_TEXTURE_GRID_GREEN = SPR_FLOOR_TEXTURE_GRASS + 11,
	SPR_FLOOR_TEXTURE_SAND_DARK = SPR_FLOOR_TEXTURE_GRASS + 12,
	SPR_FLOOR_TEXTURE_SAND_LIGHT = SPR_FLOOR_TEXTURE_GRASS + 13,

	SPR_WALL_TEXTURE_ROCK = 5593,
	SPR_WALL_TEXTURE_WOOD_RED = SPR_WALL_TEXTURE_ROCK + 1,
	SPR_WALL_TEXTURE_WOOD_BLACK = SPR_WALL_TEXTURE_ROCK + 2,
	SPR_WALL_TEXTURE_ICE = SPR_WALL_TEXTURE_ROCK + 3,

	SPR_TOOLBAR_PAUSE = 5597,
	SPR_TOOLBAR_FILE = 5599,
	SPR_TOOLBAR_ZOOM_OUT = 5601,
	SPR_TOOLBAR_ZOOM_IN = 5604,
	SPR_TOOLBAR_ROTATE = 5607,
	SPR_TOOLBAR_LAND = 5609,
	SPR_TOOLBAR_MAP = 5611,
	SPR_TOOLBAR_GUESTS = 5613,
	SPR_TOOLBAR_SCENERY = 5615,
	SPR_TOOLBAR_WATER = 5617,
	SPR_TOOLBAR_RIDES = 5619,
	SPR_TOOLBAR_VIEW = 5621,
	SPR_TOOLBAR_FOOTPATH = 5623,
	SPR_TOOLBAR_STAFF = 5627,
	SPR_TOOLBAR_PARK = 5629,
	SPR_TOOLBAR_CONSTRUCT_RIDE = 5631,
	SPR_TOOLBAR_CLEAR_SCENERY = 5633,

	SPR_NEXT_WEATHER = 23189,
	SPR_WEATHER_SUN = 23190,
	SPR_WEATHER_SUN_CLOUD = 23191,
	SPR_WEATHER_CLOUD = 23192,
	SPR_WEATHER_LIGHT_RAIN = 23193,
	SPR_WEATHER_HEAVY_RAIN = 23194,
	SPR_WEATHER_STORM = 23195,
	SPR_WEATHER_UNKNOWN = 23196,

	SPR_RATING_LOW = 23197,
	SPR_RATING_HIGH = 23198,

	SPR_MENU_NEW_GAME = 23207,
	SPR_MENU_LOAD_GAME = 23208,
	SPR_MENU_TUTORIAL = 23209,
	SPR_MENU_EXIT = 23210,
	SPR_MENU_TOOLBOX = 23211,
	SPR_MENU_LOGO = 23212,
	SPR_CREDITS_LOGO_SMALL = 23213,
	SPR_CREDITS_CHRIS_SAWYER_SMALL = 23214,
	SPR_INTRO_CHRIS_SAWYER_00 = 23215,
	SPR_INTRO_CHRIS_SAWYER_10 = SPR_INTRO_CHRIS_SAWYER_00 + 1,
	SPR_INTRO_LOGO_00 = 23218,
	SPR_INTRO_LOGO_10 = SPR_INTRO_LOGO_00 + 1,
	SPR_INTRO_LOGO_20 = SPR_INTRO_LOGO_00 + 2,
	SPR_INTRO_LOGO_01 = SPR_INTRO_LOGO_00 + 3,
	SPR_INTRO_LOGO_11 = SPR_INTRO_LOGO_00 + 4,
	SPR_INTRO_LOGO_21 = SPR_INTRO_LOGO_00 + 5,

	SPR_SIX_FLAGS = 23225,
	SPR_INTRO_INFOGRAMES_00 = 23226,
	SPR_INTRO_INFOGRAMES_10 = SPR_INTRO_INFOGRAMES_00 + 1,
	SPR_INTRO_INFOGRAMES_01 = SPR_INTRO_INFOGRAMES_00 + 2,
	SPR_INTRO_INFOGRAMES_11 = SPR_INTRO_INFOGRAMES_00 + 3,
	SPR_CREDITS_INFOGRAMES = 23230,

	SPR_G2_BEGIN = 324288,
	SPR_G2_LOGO = SPR_G2_BEGIN + 0,
	SPR_G2_TITLE = SPR_G2_BEGIN + 1,
	SPR_G2_FASTFORWARD = SPR_G2_BEGIN + 2,
	SPR_G2_SPEED_ARROW = SPR_G2_BEGIN + 3,
	SPR_G2_HYPER_ARROW = SPR_G2_BEGIN + 4,
	SPR_G2_TAB_TWITCH = SPR_G2_BEGIN + 5,
	SPR_G2_TAB_LAND = SPR_G2_BEGIN + 6,

	SPR_G2_PLACEHOLDER = SPR_G2_BEGIN + 7,

	SPR_G2_ZOOM_IN = SPR_G2_BEGIN + 8,
	SPR_G2_ZOOM_IN_DISABLED = SPR_G2_BEGIN + 9,
	SPR_G2_ZOOM_OUT = SPR_G2_BEGIN + 10,
	SPR_G2_ZOOM_OUT_DISABLED = SPR_G2_BEGIN + 11,

	SPR_G2_TAB_TREE = SPR_G2_BEGIN + 12,
	SPR_G2_TAB_PENCIL = SPR_G2_BEGIN + 13,
	SPR_G2_BUTTON_LARGE_SCENERY = SPR_G2_BEGIN + 14,
	SPR_G2_BUTTON_TREES = SPR_G2_BEGIN + 15,
	SPR_G2_BUTTON_FOOTPATH = SPR_G2_BEGIN + 16,

	SPR_G2_RCT1_CLOSE_BUTTON_0 = SPR_G2_BEGIN + 17,
	SPR_G2_RCT1_CLOSE_BUTTON_1 = SPR_G2_BEGIN + 18,
	SPR_G2_RCT1_CLOSE_BUTTON_2 = SPR_G2_BEGIN + 19,
	SPR_G2_RCT1_CLOSE_BUTTON_3 = SPR_G2_BEGIN + 20,
	SPR_G2_RCT1_TEST_BUTTON_0 = SPR_G2_BEGIN + 21,
	SPR_G2_RCT1_TEST_BUTTON_1 = SPR_G2_BEGIN + 22,
	SPR_G2_RCT1_TEST_BUTTON_2 = SPR_G2_BEGIN + 23,
	SPR_G2_RCT1_TEST_BUTTON_3 = SPR_G2_BEGIN + 24,
	SPR_G2_RCT1_OPEN_BUTTON_0 = SPR_G2_BEGIN + 25,
	SPR_G2_RCT1_OPEN_BUTTON_1 = SPR_G2_BEGIN + 26,
	SPR_G2_RCT1_OPEN_BUTTON_2 = SPR_G2_BEGIN + 27,
	SPR_G2_RCT1_OPEN_BUTTON_3 = SPR_G2_BEGIN + 28,

	SPR_G2_TITLE_RESTART = SPR_G2_BEGIN + 29,
	SPR_G2_TITLE_STOP = SPR_G2_BEGIN + 30,
	SPR_G2_TITLE_PLAY = SPR_G2_BEGIN + 31,
	SPR_G2_TITLE_SKIP = SPR_G2_BEGIN + 32,
};

#endif
