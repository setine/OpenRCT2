/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

// OpenRCT2 Scripting API definition file

// To enable IntelliSense for your scripts in Visual Studio or Visual Studio Code,
// add the following line to the top of your script and change the path appropriately.
//
//   /// <reference path="/path/to/openrct2.d.ts" />
//

export interface Console {
    clear(): void;
    log(message?: any, ...optionalParams: any[]): void;
}

export interface Configuration {
    getAll(pattern: string): { [name: string]: any };
    get<T>(key: string): T | undefined;
    get<T>(key: string, defaultValue: T): T;
    set<T>(key: string, value: T): void;
    has(key: string): boolean;
}

export interface Context {
    /**
     * The user's current configuration.
     */
    configuration: Configuration;

    /**
     * Registers a new intent (command) that can be mapped to a shortcut.
     */
    registerIntent(desc: IntentDesc);

    /**
     * Subscribes to the given hook.
     */
    subscribe(hook: string, callback: Function): IDisposable;
}

export interface IntentDesc
{
    key: string;
    title?: string;
    shortcut?: string;
    action: Function;
}

export interface IDisposable {
    dispose(): void;
}

export type TileElementType =
    "surface" | "footpath-item";

export interface TileElement {
    type: TileElementType;
    zBase: number;
    zClearance: number;

    // footpath-item:
    broken: boolean;
}

export interface Tile {
    x: number;
    y: number;
    type: TileType;
    elements: TileElement[];
}

export interface Ride {
    name: string;
    excitement: number;
    intensity: number;
    nausea: number;
    totalCustomers: number;
}

export type TileElementType =
    "car" | "duck" | "peep";

export interface Thing {
    type: ThingType;
    x: number;
    y: number;
    z: number;

    // Peep
    tshirt: number;
    trousers: number;
}

export interface Map {
    size: { x: number; y: number; };
    rides: number;
    things: number;

    getRide(id: number): Ride;
    getTile(x: number, y: number): Tile;
    getThing(id: number): Thing;
}

export type ParkMessageType =
    "attraction" | "peep_on_attraction" | "peep" | "money" | "blank" | "research" | "guests" | "award" | "chart";

export interface ParkMessage {
    type: ParkMessageType;
    text: string;
}

export interface Park {
    cash: number;
    rating: number;
    bankLoan: number;
    maxBankLoan: number;

    postMessage(message: ParkMessage): void;
}

export interface Window {
    id: number;
    classification: string;
    x: number;
    y: number;
    width: number;
    height: number;
    title: string;
    isSticky: boolean;

    close(): void;
}

export interface WindowDesc {
    classification: string;
    x?: number;
    y?: number;
    width: number;
    height: number;
    title: string;
    id?: number;
    minWidth?: number;
    minHeight?: number;
}

export interface Ui {
    width: number;
    height: number;
    windows: number;

    getWindow(id: number): Window;
    getWindow(classification: string, id?: number): Window;
    openWindow(desc: WindowDesc): Window;
    closeWindows(classification: string, id?: number): void;
    closeAllWindows(): void;
}

declare global {
    var console: Console;
    var context: Context;
    var map: Map;
    var park: Park;
    var ui: Ui;
}