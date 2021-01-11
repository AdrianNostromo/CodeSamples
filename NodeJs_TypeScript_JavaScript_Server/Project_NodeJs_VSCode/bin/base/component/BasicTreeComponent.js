"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LogicError_1 = require("base/exception/LogicError");
class BasicTreeComponent {
    constructor() {
        this.componentsList = [];
        this.isCreated = false;
        this.isDisposed = false;
        this.root = null;
        this.parent = null;
        //void
    }
    getComponentsList() {
        return this.componentsList;
    }
    getRoot() {
        return this.root;
    }
    setRoot(root) {
        this.root = root;
    }
    getParent() {
        return this.parent;
    }
    setParent(parent) {
        this.parent = parent;
    }
    reservedCreate() {
        if (this.isCreated) {
            throw new LogicError_1.LogicError("BaseAppComponent.x_dispose.1.");
        }
        this.isCreated = true;
        this.create_framework();
        this.create_content();
        this.create_post();
    }
    create_framework() {
        //void
    }
    create_content() {
        //void
    }
    create_post() {
        //void
    }
    addComponent(component) {
        if (component == null || this.componentsList.indexOf(component) >= 0) {
            throw new Error("Error. 1.");
        }
        if (this.root == null) {
            // This component must be on the components tree.
            throw new Error("Error. 1.");
        }
        if (component.getRoot() != null) {
            // Child component must not be on the components tree.
            throw new Error("Error. 1.");
        }
        component.setRoot(this.root);
        component.setParent(this);
        this.componentsList.push(component);
        component.reservedCreate();
        this.onSubcomponentAdded(component);
    }
    onSubcomponentAdded(component) {
        //void
    }
    onSubcomponentRemoving_pre(component) {
        //void
    }
    disposeComponent(component) {
        if (this.root == null) {
            throw new Error("Error. 1.");
        }
        // "root" doesn't have a "parent".
        if (this.parent == null && this != this.root) {
            throw new Error("Error. 1.");
        }
        if (component.getRoot() == null) {
            throw new Error("Error. 1.");
        }
        if (component.getParent() != this) {
            throw new Error("Error. 1.");
        }
        this.onSubcomponentRemoving_pre(component);
        let index = this.componentsList.indexOf(component);
        if (index < 0) {
            throw new Error("Error.");
        }
        component.dispose();
        component.setRoot(null);
        component.setParent(null);
        this.componentsList.splice(index, 1);
    }
    getIsDisposed() {
        return this.isDisposed;
    }
    dispose() {
        if (this.isDisposed) {
            throw new LogicError_1.LogicError("BaseAppComponent.dispose.1.");
        }
        this.isDisposed = true;
        //void
    }
}
exports.BasicTreeComponent = BasicTreeComponent;
//# sourceMappingURL=BasicTreeComponent.js.map