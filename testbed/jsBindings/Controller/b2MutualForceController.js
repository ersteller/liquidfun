
/**@constructor*/
function b2MutualForceController() {
    /* we need to create the mfc cpp instance so we have the pointer for later reference*/
    //this.ptr = ptr;
    this.ptr = b2MutualForceController_Create();
    // this.buffer = DataView(Module.HEAPU8.buffer, this.ptr);
}
  
var b2MutualForceController_Create = 
    Module.cwrap('b2MutualForceController_Create', 'number', []);
var b2MutualForceController_Step =
    Module.cwrap('b2MutualForceController_Step', 'null', ['number']);
var b2MutualForceController_AddGroup =
    Module.cwrap('b2MutualForceController_AddGroup', 'null', ['number', 'number']);

b2MutualForceController.prototype.Step = function() {
    b2MutualForceController_Step(this.ptr);
};

b2MutualForceController.prototype.AddGroup = function(p) {
    b2MutualForceController_AddGroup(this.ptr, p.ptr);
};
  