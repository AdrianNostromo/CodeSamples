"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicTreeComponent_1 = require("./BasicTreeComponent");
const LoopListTypes_1 = require("./loop/config/LoopListTypes");
const InterfaceUtil_1 = require("base/util/InterfaceUtil");
const LogicError_1 = require("base/exception/LogicError");
class BasicLoopComponent extends BasicTreeComponent_1.BasicTreeComponent {
    constructor() {
        super();
        // Looping chained list.
        this.prevLoopUserNodesList = [];
        // Looping chained list.
        this.nextLoopUserNodesList = [];
        // Contains a loop user if one is active.
        // It can be this.
        // If this is not a loop user, it can be a sub component that uses the specific loop.
        this.floatingLoopUsersList = [];
        //void
    }
    getPrevLoopUserNode(loopIndex) {
        return this.prevLoopUserNodesList[loopIndex];
    }
    setPrevLoopUserNode(loopIndex, loopUserNode) {
        return (this.prevLoopUserNodesList[loopIndex] = loopUserNode);
    }
    insertPrevLoopUserNode(loopIndex, loopUserNode) {
        loopUserNode.setPrevLoopUserNode(loopIndex, this.getPrevLoopUserNode(loopIndex));
        this.getPrevLoopUserNode(loopIndex).setNextLoopUserNode(loopIndex, loopUserNode);
        loopUserNode.setNextLoopUserNode(loopIndex, this);
        this.setPrevLoopUserNode(loopIndex, loopUserNode);
        return loopUserNode;
    }
    getNextLoopUserNode(loopIndex) {
        return this.nextLoopUserNodesList[loopIndex];
    }
    setNextLoopUserNode(loopIndex, loopUserNode) {
        return (this.nextLoopUserNodesList[loopIndex] = loopUserNode);
    }
    insertNextLoopUserNode(loopIndex, loopUserNode) {
        loopUserNode.setNextLoopUserNode(loopIndex, this.getNextLoopUserNode(loopIndex));
        this.getNextLoopUserNode(loopIndex).setPrevLoopUserNode(loopIndex, loopUserNode);
        loopUserNode.setPrevLoopUserNode(loopIndex, this);
        this.setNextLoopUserNode(loopIndex, loopUserNode);
        return loopUserNode;
    }
    removeLoopUserNode(loopIndex) {
        this.getPrevLoopUserNode(loopIndex).setNextLoopUserNode(loopIndex, this.getNextLoopUserNode(loopIndex));
        this.getNextLoopUserNode(loopIndex).setPrevLoopUserNode(loopIndex, this.getPrevLoopUserNode(loopIndex));
        this.setPrevLoopUserNode(loopIndex, this);
        this.setNextLoopUserNode(loopIndex, this);
    }
    descendantLoopUserAdded(loopIndex, component) {
        if (this.floatingLoopUsersList[loopIndex] == null) {
            this.floatingLoopUsersList[loopIndex] = component;
            if (this != this.getRoot()) {
                this.getParent().descendantLoopUserAdded(loopIndex, component);
            }
        }
        else {
            if (component == this && this.floatingLoopUsersList[loopIndex] != this) {
                // A child component hijacked the value  before "this" was able to set it.
                // this.floatingLoopUsersList[loopIndex] != this
                throw new LogicError_1.LogicError("Error. 1.");
            }
            if (this.floatingLoopUsersList[loopIndex] == this) {
                // "this" is the floating loop user.
                // Add the descendant loop user as a prev.
                this.floatingLoopUsersList[loopIndex].insertPrevLoopUserNode(loopIndex, component);
            }
            else {
                // The current float loop user is a descendant of "this".
                // Add the descendant loop user as a next.
                this.floatingLoopUsersList[loopIndex].insertNextLoopUserNode(loopIndex, component);
                // Also save the descendant as the floating loop user.
                this.floatingLoopUsersList[loopIndex] = component;
            }
        }
    }
    descendantLoopUserRemovingPre(loopIndex, component) {
        // Remove the component from "floatingLoopUsersList" if it is in the list.
        // The "floatingLoopUsersList" list may contain a descendant if "this" doesn't use a loop.
        // If in the list, also call this method on the parent.
        if (this.floatingLoopUsersList[loopIndex] == component) {
            this.floatingLoopUsersList[loopIndex] = null;
            if (this != this.getRoot()) {
                this.getParent().descendantLoopUserRemovingPre(loopIndex, component);
            }
        }
    }
    getFloatingLoopUser(loopIndex) {
        return this.floatingLoopUsersList[loopIndex];
    }
    create_framework() {
        super.create_framework();
        if (InterfaceUtil_1.InterfaceUtil.implementsInterface(this, "ILoopUserBase_General")) {
            this.prevLoopUserNodesList[LoopListTypes_1.LoopListTypes.LOOP_INDEX_GENERAL] = this;
            this.nextLoopUserNodesList[LoopListTypes_1.LoopListTypes.LOOP_INDEX_GENERAL] = this;
        }
        if (InterfaceUtil_1.InterfaceUtil.implementsInterface(this, "ILoopUserBase_Eco")) {
            this.prevLoopUserNodesList[LoopListTypes_1.LoopListTypes.LOOP_INDEX_ECO] = this;
            this.nextLoopUserNodesList[LoopListTypes_1.LoopListTypes.LOOP_INDEX_ECO] = this;
        }
        if (InterfaceUtil_1.InterfaceUtil.implementsInterface(this, "ILoopUserBase_GameWorld")) {
            this.prevLoopUserNodesList[LoopListTypes_1.LoopListTypes.LOOP_INDEX_GAME_WORLD] = this;
            this.nextLoopUserNodesList[LoopListTypes_1.LoopListTypes.LOOP_INDEX_GAME_WORLD] = this;
        }
        if (InterfaceUtil_1.InterfaceUtil.implementsInterface(this, "ILoopUserBase_General")) {
            this.descendantLoopUserAdded(LoopListTypes_1.LoopListTypes.LOOP_INDEX_GENERAL, this);
        }
        if (InterfaceUtil_1.InterfaceUtil.implementsInterface(this, "ILoopUserBase_Eco")) {
            this.descendantLoopUserAdded(LoopListTypes_1.LoopListTypes.LOOP_INDEX_ECO, this);
        }
        if (InterfaceUtil_1.InterfaceUtil.implementsInterface(this, "ILoopUserBase_GameWorld")) {
            this.descendantLoopUserAdded(LoopListTypes_1.LoopListTypes.LOOP_INDEX_GAME_WORLD, this);
        }
    }
    dispose() {
        if (InterfaceUtil_1.InterfaceUtil.implementsInterface(this, "ILoopUserBase_General")) {
            this.descendantLoopUserRemovingPre(LoopListTypes_1.LoopListTypes.LOOP_INDEX_GENERAL, this);
        }
        if (InterfaceUtil_1.InterfaceUtil.implementsInterface(this, "ILoopUserBase_Eco")) {
            this.descendantLoopUserRemovingPre(LoopListTypes_1.LoopListTypes.LOOP_INDEX_ECO, this);
        }
        if (InterfaceUtil_1.InterfaceUtil.implementsInterface(this, "ILoopUserBase_GameWorld")) {
            this.descendantLoopUserRemovingPre(LoopListTypes_1.LoopListTypes.LOOP_INDEX_GAME_WORLD, this);
        }
        super.dispose();
    }
}
exports.BasicLoopComponent = BasicLoopComponent;
//# sourceMappingURL=BasicLoopComponent.js.map