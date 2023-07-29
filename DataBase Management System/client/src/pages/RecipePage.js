import {useState, useEffect } from 'react';
import {useParams} from 'react-router-dom';
import Comment from "../components/Comments";
import '../styles/pageStyle.css';

const RecipePage = () => {
    //still need to do: 
    //getting and seting comments
    //getting and setting ratings
    const {id} = useParams();
    const axios= require("axios").default;

    const [name, setName]= useState("");
    const [ingredients, setIngredients]= useState([]);
    //const [nutritionValue, setNV]= useState([]);
    const [cooking_method, setCM]= useState([]);
    const [allergy, setAllergy]= useState([]);
    const [country, setOC]= useState([]);
    const [cooking_equipment, setCE]= useState([]);

    const [avgRating, setavgRating]= useState(0);
    const [popular, setPopular]= useState(0);
    const [userRate, setUserrate]= useState(0);
    const [commentContent, setComment]= useState("");

    useEffect(() => {
        console.log("Certain recipe");

        axios.get("http://localhost:7000/api/cookingMethod", {
            params: {
                id: Number(id)
            }
        }) //need to send request body
        .then((res) => {
            //console.log(res.data);
            setCM(res.data);
        }).catch((err) => {

        });

        axios.get("http://localhost:7000/api/recipe_name", {
            params: {
                id: Number(id)
            }
        }).then((res) => {
            //console.log(res.data);
            setName(res.data[0].recipe_name);
        }).catch((err) => {

        });

        axios.get("http://localhost:7000/api/recipe_ingredients", {
            params: {
                id: Number(id)
            }
        }) //need to send request body
        .then((res) => {
            setIngredients(res.data);
        }).catch((err) => {

        });

        axios.get("http://localhost:7000/api/recipe_country", {
            params: {
                id: Number(id)
            }
        }) //need to send request body
        .then((res) => {
            //console.log("country", res.data);
            setOC(res.data);
        }).catch((err) => {

        });

        axios.get("http://localhost:7000/api/recipe_allergy", {
            params: {
                id: Number(id)
            }
        }) //need to send request body
        .then((res) => {
            //console.log(res.data);
            setAllergy(res.data);
        }).catch((err) => {

        });

        //axios.get avg rating
        axios.get("http://localhost:7000/api/get_recipe_ratings", {
            params: {
                id: Number(id)
            }
        }).then((res) => {
            const toNumRating= res.data[0].score //------------------------------------uh
            setavgRating(toNumRating);
            setPopular(res.data[0].popularity);
        }).catch((err) => {

        });

        axios.get("http://localhost:7000/api/cookingEquipment", {
            params: {
                id: Number(id)
            }
        }) //need to send request body
        .then((res) => {
            setCE(res.data);
        }).catch((err) => {

        });

    }, []);


    const rated = (e) => {
        
        //console.log("user rate", Number(e.target.value));
        var amount= 0;
        axios.get("http://localhost:7000/api/get_recipe_review", {
            params: {
                id: id
            }
        }).then((res) => {
            amount= res.data.length;
            console.log("length", amount);
        }).catch((err) => {

        });

        const newPopularity= popular+1;
        const newRating= Math.ceil(((avgRating*amount)+Number(e.target.value))/(amount+1));
        console.log("new rating", newRating);
        //axios.post the rating
        axios.put("http://localhost:7000/api/update_recipe_ratings", {
            recipe_id: id,
            pop: newPopularity,
            score: newRating 
        }).then((res) => {
            //setavgRating(res.data[0].score);
            //console.log("put grab new", res);
        }).catch((err) => {

        });
        setUserrate(Number(e.target.value));
        
    }

    useEffect(() => {
        //console.log("new rating");
        
        axios.get("http://localhost:7000/api/get_recipe_ratings", {
            params: {
                id: Number(id)
            }
        }).then((res) => {
            const toNumRating= res.data[0].score //------------------------------------uh
            console.log("grab new", toNumRating);
            setavgRating(toNumRating);
        }).catch((err) => {

        });
        
    }, [userRate]);

    
    const postComment = () => {
        const randid= 11 + parseInt(Math.random() * (1000-11));
        const randscore= 1 + parseInt(Math.random() * (5-1));

        axios.post("http://localhost:7000/api/set_recipe_review", {
            comment_id: randid,
            recipe_id: id,
            content: commentContent,
            score: randscore
        }).then((res) => {

        }).catch((err) => {

        });
        
    }
    
    return (
        <div> 
            <div>

                <h1 key= {name}> {name} </h1>

                <h2> Country of origin: {
                    country.map((item) => {
                        return (
                            <span key= {item.origin_country}> {item.origin_country} </span>
                        );
                    })
                } </h2>
                
                <h2> Possible allergies: </h2> 
                {
                    allergy.map((item) => {
                        return (
                            <p key={item.allergy}> {item.allergy} </p>
                        );
                    })
                }

                <h2> Some cooking equipment you will need are: </h2>

                {
                    cooking_equipment.map((item) => {
                        return (
                            <p> {item.equipment_name} </p>
                        );
                    })
                }

                <h2> Ingredients </h2>

                {
                    ingredients.map((item) => {
                        return (
                            <p key={item.content}> {item.amount} {item.content} </p>
                        );
                    })
                }

                <h2> Directions: </h2>

                {//recipe id, content
                    cooking_method.map((item) => {
                        return (
                            <p key={item.content}> {item.content} </p>
                        );
                    })
                }

            </div>

            <div>
                <h3> Rating {avgRating} </h3>
                <div className="rating">
                    <button className="rate" value={1} onClick={rated}> 1 </button>
                    <button className="rate" value={2} onClick={rated}> 2 </button>
                    <button className="rate" value={3} onClick={rated}> 3 </button>
                    <button className="rate" value={4} onClick={rated}> 4 </button>
                    <button className="rate" value={5} onClick={rated}> 5 </button>
                </div>
                <h3> Comments </h3>
                <Comment id={id}/>
            </div>

            <div className="commenting">
                <label> Make a comment </label> 
                <input className= "comment" type="text" name="comment" onChange={e => setComment(e.target.value)}/>
                <button onClick={postComment}> Comment </button>
            </div>
            
        </div>
    );
}

export default RecipePage;