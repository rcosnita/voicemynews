((module, exports) => {
class Test {
    constructor(width, height) {
        this.height = height;
        this.width = width;
    }
}

module.exports = Test;
return module;})(module, exports);