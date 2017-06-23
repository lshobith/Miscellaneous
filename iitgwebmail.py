import poplib
serv = poplib.POP3_SSL( 'tamdil.iitg.ernet.in' , '995' )    # replace tamdil with your IITG Webmail server
serv.user( 'username' )
serv.pass_( 'password' )
