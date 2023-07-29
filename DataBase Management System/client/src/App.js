import React from 'react';
import {
  BrowserRouter as Router,
  Routes,
  Route,
} from 'react-router-dom';
import Home from './pages/Home.js';
import Recipes from './pages/Recipes.js';
import RecipePage from './pages/RecipePage.js';
import Nav from './components/Nav.js';
import Signin from "./pages/Signin.js";
import './App.css';

function App() {
  return ( 
      <Router>
        <Nav />
        <Routes>
          <Route path= "/" element= {<Home/>} exact/>
          <Route path= "signin" element= {<Signin/>} exact/>
          <Route path="/recipes" element= {<Recipes/>} exact />
          <Route path= "/recipe/:id" element= {<RecipePage/>} exact />
        </Routes>
      </Router>
    
    
  );
}

export default App;
