import React, {useState, useEffect} from 'react';
import {Link} from 'react-router-dom';
import '../styles/pageStyle.css'

const Home= () => {
    //still need to do recipe of the month
    const [id, setId]= useState(0);
    const [name, setName]= useState("");
    const [loaded, setLoaded]= useState(false);
    const axios= require('axios').default;

    useEffect(() => {
        console.log("Home page");
        
        axios.get("http://localhost:7000/api/highest_ranked")
        .then((res) => {
            setId(res.data[0].recipe_id);
        }).catch((err) => {

        });

    }, []);

    useEffect(() => {
        axios.get("http://localhost:7000/api/recipe_name", {
            params: {
                id: id
            }
        }).then((res) => {
            setName(res.data[0].recipe_name);
            setLoaded(true);
        }).catch((err) => {

        });
    }, [id]);

    return (
        <div>
            <h1> Welcome, find something to make </h1>
            <Link to ={`/recipe/${id}`}> 
                {loaded ? <h1 key= {id}> Recipe of the month: {name} </h1>: "...loading" }
            </Link> 
        </div>
    );
    
}

export default Home;