import React from 'react';
import {Link} from 'react-router-dom'
import '../App.css';
import start from '../img/R.jfif';

const Nav= () => {
    return (
        <div> 
            <header className="App-header">
                <img src={start} className="App-start" alt="logo" />
                <h1> Welcome </h1>
            </header>
      
            <ul className="bar"> 
                <Link to= "/"> <li className='list'> Home </li> </Link>
                <Link to= "/recipes"> <li className='list'> Recipe </li> </Link> 
                
                <Link to="/signin" className="NavButton"> <li className='list'> Sign in</li> </Link>
            </ul>
      </div>
    );
}

export default Nav;