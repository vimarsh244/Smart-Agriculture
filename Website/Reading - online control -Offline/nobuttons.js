document.onkeydown = function(event) {
		switch (event.keyCode) {
		   case 65:
                publish('2','SA/mc',2);
						break;
		   case 87:
                publish('1','SA/mc',2);
			 		  break;
		   case 68:
                publish('4','SA/mc',2);
			  		break;
		   case 83:
                publish('5','SA/mc',2);
              break;
        case 88:
                publish('3','SA/mc',2);
			  		break;
	}
};
